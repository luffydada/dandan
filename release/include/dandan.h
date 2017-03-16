/***************************************************************************
*    File name:        dandan.h
*    Description:
*    Create date:      2016-11-20 21:37
*    Modified date:    2016-11-20 21:37
*    Version:          V1.0
*    Author:           wangying
***************************************************************************/
#ifndef __dandan_h__
#define __dandan_h__
#define DANDAN_EXPORT
#define DANDAN_EXPORT_API extern "C" DANDAN_EXPORT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <map>

#include "type/dd_type.h"

#include "utility/dd_character.h"
#include "utility/dd_filemanager.h"
#include "utility/dd_log.h"
#include "utility/dd_mediaplayer.h"
#include "utility/dd_playlist.h"
#include "utility/dd_reg.h"

#include "service/dd_command.h"
#include "service/dd_device.h"
#include "service/dd_devsrvmanager.h"
#include "service/dd_service.h"

#include "kernel/dd_app.h"
#include "kernel/dd_mediadevice.h"
#include "kernel/dd_process.h"
#include "kernel/dd_thread.h"
#include "kernel/dd_timer.h"

#endif //dandan.h
