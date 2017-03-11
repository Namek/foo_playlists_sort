#pragma once


/**
 * Includes
 */
#include <windows.h>
#include <string>
#include <tchar.h>
#include <strsafe.h>
#include <SDK/foobar2000.h>
#include <helpers/helpers.h>


/**
 * Some settings
 */
	#define FOO_CLASS L"{A7E2FFAA-C0B3-4b8e-A693-E47881E89F81}"


/**
 * Functions
 */

	enum SORT_TYPE
	{
		ASC = 0,
		DESC,
		TIME_ASC,
		TIME_DESC,
		TRACKS_ASC,
		TRACKS_DESC
	};

	GUID GetFooGUID(int seed);
	void SortPlaylists(SORT_TYPE);
