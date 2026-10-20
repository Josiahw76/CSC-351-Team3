struct iNode {
	uint16_t mode,
	uint16_t nlinks,
	uint16_t uid,
	uint16_t gid,
	uint32_t size,
	uint16_t addr,
	uint8_t gen,
	uint16_t atime,
	uint16_t mtime,
	uint16_t ctime,
	uint16_t btime;
};
