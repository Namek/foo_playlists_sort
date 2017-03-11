#include "main.h"

GUID GetFooGUID(int seed)
{
	GUID conf_guid;

	unsigned char Data4[8] = "PL_Sort";
	conf_guid.Data1 = 0x12141396;
	conf_guid.Data2 = 0x9631;
	conf_guid.Data3 = seed;
	memcpy(&conf_guid.Data4, Data4, 8);

	return conf_guid;
}
