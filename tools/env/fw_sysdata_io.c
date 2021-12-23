
#include <compiler.h>
#include <errno.h>
#include <env_flags.h>
#include <fcntl.h>
#include <libgen.h>
#include <linux/fs.h>
#include <linux/stringify.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#ifdef MTD_OLD
# include <stdint.h>
# include <linux/mtd/mtd.h>
#else
# define  __user	/* nothing */
# include <mtd/mtd-user.h>
#endif

#include <sysdata.h>
#include "fw_sysdata.h"


#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))

#define min(x, y) ({				\
	typeof(x) _min1 = (x);			\
	typeof(y) _min2 = (y);			\
	(void) (&_min1 == &_min2);		\
	_min1 < _min2 ? _min1 : _min2; })


typedef struct sysdev {
	const char *devname;		/* Device name */
	long long  devoff;		    /* Device offset */
	ulong      env_size;		/* environment size */
	ulong      erase_size;		/* device erase size */
	ulong      env_sectors;		/* number of environment sectors */
	uint8_t    mtd_type;		/* type of the MTD device */
	int        is_ubi;			/* set if we use UBI volume */
} sysdev_t;


static sysdev_t sysdevice[1] = {
	{
		.devname = "/dev/mmcblk1",
		.devoff  = (890 * 1024),
		.env_size = SYSTEMDATA_SIZE,
		.erase_size = 0,
		.env_sectors = 0,
		.mtd_type = MTD_ABSENT,
		.is_ubi = 0,
		
	},
};

static int dev_current = 0;

#define DEVNAME(i)     sysdevice[(i)].devname
#define DEVOFFSET(i)   sysdevice[(i)].devoff
#define DATASIZE(i)    sysdevice[(i)].env_size
#define DEVESIZE(i)    sysdevice[(i)].erase_size
#define DATASECTORS(i) sysdevice[(i)].env_sectors
#define DEVTYPE(i)     sysdevice[(i)].mtd_type
#define IS_UBI(i)      sysdevice[(i)].is_ubi

#define CUR_SYSDATASIZE DATASIZE(dev_current)

static unsigned long usable_sysdatasize;
#define SYSDATA_SIZE      usable_sysdatasize


typedef struct global_data {
    void       *image;
    sysdata_t  sysdata; 
} global_data_t;

static global_data_t gdata;

/**
 * data_end() - compute offset of first byte right after data
 * @dev - index of enviroment buffer
 * Return:
 *  device offset of first byte right after data
 */
off_t data_end( int dev ) {
	/* environment is block aligned */
	return DEVOFFSET(dev) + DATASECTORS(dev) * DEVESIZE(dev);
}



 /* Test for bad block on NAND, just returns 0 on NOR, on NAND:
 * 0	- block is good
 * > 0	- block is bad
 * < 0	- failed to test
 */
static int flash_bad_block( int fd, uint8_t mtd_type, loff_t blockstart ) {
	if ( mtd_type == MTD_NANDFLASH)  {
		int badblock = ioctl( fd, MEMGETBADBLOCK, &blockstart );

		if ( badblock < 0 ) {
			perror( "Cannot read bad block mark" );
			return badblock;
		}

		if ( badblock ) {
#ifdef DEBUG
			fprintf( stderr, "Bad block at 0x%llx, skipping\n",
				(unsigned long long)blockstart );
#endif
			return badblock;
		}
	}

	return 0;
}


/*
 * Read data from flash at an offset into a provided buffer. On NAND it skips
 * bad blocks but makes sure it stays within DEVSECTORS (dev) starting from
 * the DEVOFFSET (dev) block. On NOR the loop is only run once.
 */
static int flash_read_buf( int dev, int fd, void *buf, size_t count, off_t offset ) {
	size_t blocklen;	/* erase / write length - one block on NAND,
				   0 on NOR */
	size_t processed = 0;	/* progress counter */
	size_t readlen = count;	/* current read length */
	off_t block_seek;	/* offset inside the current block to the start
				   of the data */
	loff_t blockstart;	/* running start of the current block -
				   MEMGETBADBLOCK needs 64 bits */
	int rc;

	blockstart = ( offset / DEVESIZE(dev) ) * DEVESIZE(dev);

	/* Offset inside a block */
	block_seek = offset - blockstart;

	if ( DEVTYPE(dev) == MTD_NANDFLASH ) {
		/*
		 * NAND: calculate which blocks we are reading. We haves
		 * to read one block at a time to skip bad blocks.
		 */
		blocklen = DEVESIZE(dev);

		/* Limit to one block for the first read */
		if (readlen > blocklen - block_seek)
			readlen = blocklen - block_seek;
	} else {
		blocklen = 0;
	}

	/* This only runs once on NOR flash */
	while  (processed < count ) {
		rc = flash_bad_block( fd, DEVTYPE(dev), blockstart );
		if ( rc < 0 )	/* block test failed */
			return -1;

		if ( blockstart + block_seek + readlen > data_end( dev ) ) {
			/* End of range is reached */
			fprintf( stderr, "Too few good blocks within range\n" );
			return -1;
		}

		if ( rc ) {	/* block is bad */
			blockstart += blocklen;
			continue;
		}

		/*
		 * If a block is bad, we retry in the next block at the same
		 * offset - see env/nand.c::writeenv()
		 */
		lseek( fd, blockstart + block_seek, SEEK_SET );

		rc = read( fd, buf + processed, readlen );
		if ( rc != readlen ) {
			fprintf( stderr, "Read error on %s: %s\n",
				DEVNAME(dev), strerror(errno) );
			return -1;
		}
#ifdef DEBUG
		fprintf( stderr, "Read 0x%x bytes at 0x%llx on %s\n",
			rc, (unsigned long long)blockstart + block_seek,
			DEVNAME(dev) );
#endif
		processed += readlen;
		readlen = min( blocklen, count - processed );
		block_seek = 0;
		blockstart += blocklen;
	}

	return processed;
}


static int flash_read( int fd, void *buf ) {
	int rc;

	if ( IS_UBI(dev_current) ) {
        return -1;
	}

	rc = flash_read_buf( dev_current, fd, buf, CUR_SYSDATASIZE,
			    DEVOFFSET(dev_current) );
	if ( rc != CUR_SYSDATASIZE )
		return -1;

	return 0;
}



/*
 * Write count bytes from begin of environment, but stay within
 * ENVSECTORS(dev) sectors of
 * DEVOFFSET (dev). Similar to the read case above, on NOR and dataflash we
 * erase and write the whole data at once.
 */
static int flash_write_buf( int dev, int fd, void *buf, size_t count ) {
	void *data;
	struct erase_info_user erase;
	size_t blocklen;	/* length of NAND block / NOR erase sector */
	size_t erase_len;	/* whole area that can be erased - may include
				   bad blocks */
	size_t erasesize;	/* erase / write length - one block on NAND,
				   whole area on NOR */
	size_t processed = 0;	/* progress counter */
	size_t write_total;	/* total size to actually write - excluding
				   bad blocks */
	off_t erase_offset;	/* offset to the first erase block (aligned)
				   below offset */
	off_t block_seek;	/* offset inside the erase block to the start
				   of the data */
	loff_t blockstart;	/* running start of the current block -
				   MEMGETBADBLOCK needs 64 bits */
	int rc;

	/*
	 * For mtd devices only offset and size of the environment do matter
	 */
	if (DEVTYPE(dev) == MTD_ABSENT) {
		blocklen = count;
		erase_len = blocklen;
		blockstart = DEVOFFSET(dev);
		block_seek = 0;
		write_total = blocklen;
	} else {
		blocklen = DEVESIZE(dev);

		erase_offset = DEVOFFSET(dev);

		/* Maximum area we may use */
		erase_len = data_end(dev) - erase_offset;

		blockstart = erase_offset;

		/* Offset inside a block */
		block_seek = DEVOFFSET(dev) - erase_offset;

		/*
		 * Data size we actually write: from the start of the block
		 * to the start of the data, then count bytes of data, and
		 * to the end of the block
		 */
		write_total = ((block_seek + count + blocklen - 1) /
			       blocklen) * blocklen;
	}

	/*
	 * Support data anywhere within erase sectors: read out the complete
	 * area to be erased, replace the environment image, write the whole
	 * block back again.
	 */
	if (write_total > count) {
		data = malloc(erase_len);
		if (!data) {
			fprintf(stderr,
				"Cannot malloc %zu bytes: %s\n",
				erase_len, strerror(errno));
			return -1;
		}

		rc = flash_read_buf(dev, fd, data, write_total, erase_offset);
		if (write_total != rc)
			return -1;

#ifdef DEBUG
		fprintf(stderr, "Preserving data ");
		if (block_seek != 0)
			fprintf(stderr, "0x%x - 0x%lx", 0, block_seek - 1);
		if (block_seek + count != write_total) {
			if (block_seek != 0)
				fprintf(stderr, " and ");
			fprintf(stderr, "0x%lx - 0x%lx",
				(unsigned long)block_seek + count,
				(unsigned long)write_total - 1);
		}
		fprintf(stderr, "\n");
#endif
		/* Overwrite the old environment */
		memcpy(data + block_seek, buf, count);
	} else {
		/*
		 * We get here, iff offset is block-aligned and count is a
		 * multiple of blocklen - see write_total calculation above
		 */
		data = buf;
	}

	if (DEVTYPE(dev) == MTD_NANDFLASH) {
		/*
		 * NAND: calculate which blocks we are writing. We have
		 * to write one block at a time to skip bad blocks.
		 */
		erasesize = blocklen;
	} else {
		erasesize = erase_len;
	}

	erase.length = erasesize;

	/* This only runs once on NOR flash and SPI-dataflash */
	while (processed < write_total) {
		rc = flash_bad_block(fd, DEVTYPE(dev), blockstart);
		if (rc < 0)	/* block test failed */
			return rc;

		if (blockstart + erasesize > data_end(dev)) {
			fprintf(stderr, "End of range reached, aborting\n");
			return -1;
		}

		if (rc) {	/* block is bad */
			blockstart += blocklen;
			continue;
		}

		if (DEVTYPE(dev) != MTD_ABSENT) {
			erase.start = blockstart;
			ioctl(fd, MEMUNLOCK, &erase);
			/* These do not need an explicit erase cycle */
			if (DEVTYPE(dev) != MTD_DATAFLASH)
				if (ioctl(fd, MEMERASE, &erase) != 0) {
					fprintf(stderr,
						"MTD erase error on %s: %s\n",
						DEVNAME(dev), strerror(errno));
					return -1;
				}
		}

		if (lseek(fd, blockstart, SEEK_SET) == -1) {
			fprintf(stderr,
				"Seek error on %s: %s\n",
				DEVNAME(dev), strerror(errno));
			return -1;
		}
#ifdef DEBUG
		fprintf(stderr, "Write 0x%llx bytes at 0x%llx\n",
			(unsigned long long)erasesize,
			(unsigned long long)blockstart);
#endif
		if (write(fd, data + processed, erasesize) != erasesize) {
			fprintf(stderr, "Write error on %s: %s\n",
				DEVNAME(dev), strerror(errno));
			return -1;
		}

		if (DEVTYPE(dev) != MTD_ABSENT)
			ioctl(fd, MEMLOCK, &erase);

		processed += erasesize;
		block_seek = 0;
		blockstart += erasesize;
	}

	if (write_total > count)
		free(data);

	return processed;
}


static int flash_open_tempfile (const char **dname, const char **target_temp ) {
	char *dup_name = strdup( DEVNAME(dev_current) );
	char *temp_name = NULL;
	int  rc = -1;

	if ( !dup_name )
		return -1;

	*dname = dirname( dup_name );
	if ( !*dname )
		goto err;

	rc = asprintf( &temp_name, "%s/XXXXXX", *dname );
	if (rc == -1)
		goto err;

	rc = mkstemp( temp_name );
	if ( rc == -1 ) {
		/* fall back to in place write */
		fprintf( stderr, "Can't create %s: %s\n", temp_name, strerror(errno) );
		free( temp_name );
	} else {
		*target_temp = temp_name;
		/* deliberately leak dup_name as dname /might/ point into
		 * it and we need it for our caller
		 */
		dup_name = NULL;
	}

err:
	if ( dup_name )
		free( dup_name );

	return rc;
}


static int flash_write( int fd_current, void *buf ) {
	int        fd_target = -1, rc, dev_target;
	const char *dname, *target_temp = NULL;
	struct     stat sb;

	if ( fstat( fd_current, &sb ) == 0 && S_ISREG(sb.st_mode) ) {
		/* if any part of flash_open_tempfile() fails we fall
			* back to in-place writes
			*/
		fd_target = flash_open_tempfile( &dname, &target_temp );
	}
	dev_target = dev_current;
	if ( fd_target == -1 )
		fd_target = fd_current;

	rc = flash_write_buf( dev_target, fd_target, buf, CUR_SYSDATASIZE );

	if ( fsync( fd_current ) && !( errno == EINVAL || errno == EROFS ) ) {
		fprintf( stderr, "fsync failed on %s: %s\n",
			DEVNAME(dev_current), strerror(errno) );
	}

	if ( fd_current != fd_target ) {
		if ( fsync( fd_target ) && ! (errno == EINVAL || errno == EROFS ) ) {
			fprintf(stderr, "fsync failed on %s: %s\n",
				DEVNAME(dev_current), strerror(errno) );
		}

		if ( close( fd_target ) ) {
			fprintf( stderr, "I/O error on %s: %s\n",
				DEVNAME(dev_target), strerror(errno) );
			rc = -1;
		}

		if ( target_temp ) {
			int dir_fd;

			dir_fd = open( dname, O_DIRECTORY | O_RDONLY );
			if ( dir_fd == -1 )
				fprintf( stderr, "Can't open %s: %s\n", dname, strerror(errno) );

			if ( rename ( target_temp, DEVNAME(dev_target) ) ) {
				fprintf( stderr, "rename failed %s => %s: %s\n",
					target_temp, DEVNAME(dev_target),
					strerror(errno) );
				rc = -1;
			}

			if ( dir_fd != -1 && fsync( dir_fd ) )
				fprintf(stderr, "fsync failed on %s: %s\n",
					dname, strerror(errno) );

			if ( dir_fd != -1 && close( dir_fd ) )
				fprintf(stderr, "I/O error on %s: %s\n",
					dname, strerror(errno) );
		}
	}
exit:
	return rc;
}


static int flash_io( int mode, void *buf ) {
	int fd_current, rc;

	/* dev_current: fd_current, erase_current */
	fd_current = open( DEVNAME(dev_current), mode );
	if ( fd_current < 0 ) {
		fprintf( stderr,
			"Can't open %s: %s\n",
			DEVNAME(dev_current), strerror(errno) );
		return -1;
	}

	if ( mode == O_RDWR ) {
		rc = flash_write( fd_current, buf );
	} else {
		rc = flash_read( fd_current, buf );
	}

	if (close( fd_current ) ) {
		fprintf( stderr,
			"I/O error on %s: %s\n",
			DEVNAME(dev_current), strerror(errno) );
		return -1;
	}

	return rc > 0 ? 0 : rc;
}


static int check_device_config( int dev ) {
	struct stat st;
	int32_t lnum = 0;
	int fd, rc = 0;

	/* Fills in IS_UBI(), converts DEVNAME() with ubi volume name */
	//ubi_check_dev(dev);

	fd = open( DEVNAME(dev), O_RDONLY );
	if ( fd < 0 ) {
		fprintf( stderr,
			"Cannot open %s: %s\n", DEVNAME(dev), strerror(errno) );
		return -1;
	}

	rc = fstat( fd, &st );
	if ( rc < 0 ) {
		fprintf( stderr, "Cannot stat the file %s\n", DEVNAME(dev) );
		goto err;
	}

	if ( IS_UBI(dev) ) {
		// rc = ioctl(fd, UBI_IOCEBISMAP, &lnum);
		// if (rc < 0) {
		// 	fprintf(stderr, "Cannot get UBI information for %s\n",
		// 		DEVNAME(dev));
		// 	goto err;
		// }
	} else if ( S_ISCHR(st.st_mode) ) {
		struct mtd_info_user mtdinfo;
		rc = ioctl( fd, MEMGETINFO, &mtdinfo );
		if ( rc < 0 ) {
			fprintf( stderr, "Cannot get MTD information for %s\n",
				DEVNAME(dev) );
			goto err;
		}
		if (mtdinfo.type != MTD_NORFLASH &&
		    mtdinfo.type != MTD_NANDFLASH &&
		    mtdinfo.type != MTD_DATAFLASH &&
		    mtdinfo.type != MTD_UBIVOLUME) {
			fprintf( stderr, "Unsupported flash type %u on %s\n",
				mtdinfo.type, DEVNAME(dev) );
			goto err;
		}
		DEVTYPE(dev) = mtdinfo.type;
		if ( DEVESIZE(dev) == 0 )
			/* Assume the erase size is the same as the env-size */
			DEVESIZE(dev) = DATASIZE(dev);
	} else {
		uint64_t size;
		DEVTYPE(dev) = MTD_ABSENT;
		if ( DEVESIZE(dev) == 0 )
			/* Assume the erase size to be 512 bytes */
			DEVESIZE(dev) = 0x200;

		/*
		 * Check for negative offsets, treat it as backwards offset
		 * from the end of the block device
		 */
		if ( DEVOFFSET(dev) < 0 ) {
			rc = ioctl( fd, BLKGETSIZE64, &size );
			if ( rc < 0 ) {
				fprintf( stderr,
					"Could not get block device size on %s\n",
					DEVNAME(dev) );
				goto err;
			}

			DEVOFFSET(dev) = DEVOFFSET(dev) + size;
#ifdef DEBUG
			fprintf( stderr,
				"Calculated device offset 0x%llx on %s\n",
				DEVOFFSET(dev), DEVNAME(dev) );
#endif
		}
	}

	if ( DATASECTORS(dev) ==  0)
		/* Assume enough sectors to cover the environment */
		DATASECTORS(dev) = DIV_ROUND_UP(DATASIZE(dev), DEVESIZE(dev));

	if ( DEVOFFSET(dev) % DEVESIZE(dev) != 0 ) {
		fprintf( stderr,
			"Environment does not start on (erase) block boundary\n" );
		errno = EINVAL;
		return -1;
	}

	if ( DATASIZE(dev) > DATASECTORS(dev) * DEVESIZE(dev) ) {
		fprintf( stderr,
			"Environment does not fit into available sectors\n" );
		errno = EINVAL;
		return -1;
	}

 err:
	close( fd );
	return rc;
}


/** 
 * _sysdata_rd( ) - simple read operation 
 * 
 * This uses the device selected at compilation time
 * 
 * @table: main data structure in which to load stored data
 * @return 0 if ok, negative value if operation fails
*/
int _sysdata_rd( sysdata_t *table ) {
	int ret;
	
	dev_current = 0;
	ret = check_device_config ( dev_current );
	if ( ret )
		return -ENODEV;

	
	if ( flash_io( O_RDONLY, (void *)table ) ) {
		return -EIO;
	} 

	return 0;
}


/** 
 * _sysdata_wr( ) - simple write operation 
 * 
 * This uses the device selected at compilation time
 * 
 * @table: main data structure to save
 * @return 0 if ok, negative value if operation fails
*/
int _sysdata_wr( sysdata_t *table ) {
	int ret;
	
	dev_current = 0;
	ret = check_device_config ( dev_current );
	if ( ret )
		return -ENODEV;

	if ( flash_io( O_RDWR, (void *)table ) ) {
		return -EIO;
	} 

	return 0;
}