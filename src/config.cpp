#include "main.h"
#include "config.h"


/**
 * Main menu commands
 */

#define TITLE "Sort Playlists"
#define OPT_ASC "Ascending"
#define OPT_DESC "Descending"
#define OPT_BYTIME_ASC "By time (ASC)"
#define OPT_BYTIME_DESC "By time (DESC)"
#define OPT_TRACKS_ASC "By tracks (ASC)"
#define OPT_TRACKS_DESC "By tracks (DESC)"


static GUID mainmenu_playlists_sort_group_guid = GetFooGUID(10);
static GUID mainmenu_playlists_sort_group_popup_guid = GetFooGUID(11);
static GUID mainmenu_playlists_sort_command_guid = GetFooGUID(12);


class mainmenu_commands_impl : public mainmenu_commands
{
	virtual t_uint32 get_command_count()
	{
		return 6;
	}

	virtual GUID get_command(t_uint32 p_index)
	{
		if (p_index <= 5)
			return GetFooGUID(p_index);

		return pfc::guid_null;
	}

	virtual void get_name(t_uint32 p_index, pfc::string_base & p_out)
	{
		if (p_index == 0)
			p_out = OPT_ASC;
		else if (p_index == 1)
			p_out = OPT_DESC;
		else if (p_index == 2)
			p_out = OPT_BYTIME_ASC;
		else if (p_index == 3)
			p_out = OPT_BYTIME_DESC;
		else if (p_index == 4)
			p_out = OPT_TRACKS_ASC;
		else if (p_index == 5)
			p_out = OPT_TRACKS_DESC;
	}

	virtual bool get_description(t_uint32 p_index, pfc::string_base & p_out)
	{
		if (p_index == 0 || p_index == 1)
			p_out = "Sorts all playlists alphabetically.";
		else if (p_index == 2 || p_index == 3)
			p_out = "Sorts all playlists by time length.";
		else if (p_index == 4 || p_index == 5)
			p_out = "Sorts all playlists by number of tracks.";

		return true;
	}

	virtual GUID get_parent()
	{
		return mainmenu_playlists_sort_group_popup_guid;
	}

	virtual void execute(t_uint32 p_index, service_ptr_t<service_base> p_callback)
	{
		if (core_api::assert_main_thread())
			if (p_index == 0)
				SortPlaylists(ASC);
			else if (p_index == 1)
				SortPlaylists(DESC);
			else if (p_index == 2)
				SortPlaylists(TIME_ASC);
			else if (p_index == 3)
				SortPlaylists(TIME_DESC);
			else if (p_index == 4)
				SortPlaylists(TRACKS_ASC);
			else if (p_index == 5)
				SortPlaylists(TRACKS_DESC);

		cfg_sort_type = p_index;
	}

	virtual bool get_display(t_uint32 p_index, pfc::string_base & p_text, t_uint32 & p_flags)
	{
		p_flags = 0;

		if (is_checked(p_index))
			p_flags |= flag_checked;

		get_name(p_index, p_text);
		return true;
	}

	bool is_checked(t_uint32 p_index)
	{
		if (advconfig_autosort)
			return (cfg_sort_type.get_value() == p_index);

		return false;
	}
};


mainmenu_commands_factory_t<mainmenu_commands_impl> playlists_sort_command;

mainmenu_group_popup_factory playlists_sort_menu(mainmenu_playlists_sort_group_popup_guid,
												 mainmenu_groups::view,
												 mainmenu_commands::sort_priority_dontcare,
												 TITLE);


/**
 * Advanced preferences
 */

static GUID advconf_playlists_sort_branch_guid = GetFooGUID(14);

advconfig_branch_factory advconfig_playlists_sort_branch (
	"Playlist Sorter",
	advconf_playlists_sort_branch_guid,
	advconfig_branch::guid_branch_tools,
	0.1
);

advconfig_checkbox_factory advconfig_autosort (
	"Sort playlists automatically",
	GetFooGUID(15),
	advconf_playlists_sort_branch_guid,
	1.0,
	true
);

advconfig_checkbox_factory advconfig_linux_like (
	"Linux-like names comparing",
	GetFooGUID(16),
	advconf_playlists_sort_branch_guid,
	0.9,
	false
);


/**
 * Internal configuration
 */

cfg_int cfg_sort_type(GetFooGUID(20), SORT_TYPE::ASC);
