#include "BlockChain.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include "RIPEMD160.h"

#ifdef WIN32
#include <conio.h>
#endif

#ifdef _MSC_VER
#pragma warning(disable:4996 4702 4505)
#endif

int main(int argc,const char **argv)
{
	uint32_t maxBlocks = 10000000;
	uint32_t searchForTextLength = 0;
	const char *dataPath = "/Users/tgebhart/Library/Application Support/Bitcoin/blocks";
	searchForTextLength = 0;
	int i = 1;
	while ( i < argc )
	{
		const char *option = argv[i];
		if ( *option == '-' )
		{
			if ( strcmp(option,"-max_blocks") == 0 )
			{
				i++;
				if ( i < argc )
				{
					maxBlocks = atoi(argv[i]);
					if ( maxBlocks < 1 )
					{
						printf("Invalid max_blocks value '%s'\n", argv[i] );
						maxBlocks = 1000000;
					}
					else
					{
						printf("Maximum blocks set to %d\r\n", maxBlocks);
					}
				}
				else
				{
					printf("Error parsing option '-max_blocks', missing block number.\n");
				}
			}
			if (strcmp(option, "-text") == 0)
			{
				i++;
				if (i < argc)
				{
					searchForTextLength = atoi(argv[i]);
					if (maxBlocks < 8 )
					{
						printf("Invalid search for text value value '%s'\n", argv[i]);
						searchForTextLength = 0;
					}
					else
					{
						printf("Search for text length %d\r\n", maxBlocks);
					}
				}
				else
				{
					printf("Error parsing option '-text', missing text length.\n");
				}
			}
			else
			{
				printf("Unknown option '%s'\n", option );
			}
		}
		else
		{
			if ( (i+1) == argc )
			{
				printf("Using directory: %s to locate bitcoin data blocks.\n", option );
				dataPath = option;
			}
			else
			{
				printf("%s not a valid option.\n", option );
			}
		}
		i++;
	}
	BlockChain *b = createBlockChain(dataPath,maxBlocks);
	if (b)
	{
		b->setSearchTextLength(searchForTextLength);
		printf("Scanning the blockchain for blocks.\r\n");
		for (;;)
		{
			uint32_t lastBlockRead;
			bool finished = b->scanBlockChain(lastBlockRead);
			if ( finished )
			{
				break;
			}
			else
			{
				if ((lastBlockRead % 1000) == 0)
				{
					printf("Scanned block header: %d\r\n", lastBlockRead);
				}
			}
		}
		printf("Finished scanning the available blocks.\r\n");
		printf("Now building the blockchain\r\n");
		uint32_t ret = b->buildBlockChain();
		printf("Found %d blocks.\r\n", ret);
		for (uint32_t i = 0; i < ret; i++)
		{
			if (((i + 1) % 100) == 0)
			{
				printf("Processing block: %d\r\n", i);
			}
			const BlockChain::Block *block = b->readBlock(i);
			if (block == nullptr)
			{
				printf("Finished reading blocks.\r\n");
				break;
			}
			else
			{
				// Print one in every 100 blocks.  Obviously you would modify this code to interpret the block data however you best see fit.
				if (((i + 1) % 100) == 0)
				{
					printf("Processing block: %d\r\n", i);
					b->printBlock(block);
				}
			}
		}
	}

	return 0;
}
