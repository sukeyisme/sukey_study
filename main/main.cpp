#include "utilities.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include "commandlineflags.h"
#include "logging.h"
#include "raw_logging.h"

int main(int argc,char *argv[])
{
	long num_cookies(999);
	LOG(INFO) << "Found " << num_cookies << " cookies";
	return 0;
}