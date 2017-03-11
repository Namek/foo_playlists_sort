#include "main.h"
#include "config.h"


DECLARE_COMPONENT_VERSION(
	"Playlist Sorter",
	"0.2.0",
	"Sort your playlists.\n"
	"To disable automatic sorting, go to:\n"
	"   File -> Preferences->Advanced->Tools->Playlist Sorter\n"
	"\n"
	"author:\n"
	" Kamil nameczanin (aka Namek) Dabrowski\n"
	" http://namek.ovhost.pl/code/foo_playlists_sort\n"
);



/********************************************************************************* /
/*    Main implementation                                                          /
/**********************************************************************************/


class playlists_resort : public main_thread_callback
{

public:

	void callback_run()
	{
		SortPlaylists((SORT_TYPE)cfg_sort_type.get_value());
	}

};

class playlists_callback : public playlist_callback_impl_base
{

public:

	playlists_callback() : playlist_callback_impl_base(
		flag_on_items_added | flag_on_items_removed | flag_on_playlist_created | flag_on_playlist_renamed)
	{}

	void on_items_added(t_size p_playlist,t_size p_start, const pfc::list_base_const_t<metadb_handle_ptr> & p_data,const bit_array & p_selection)
	{
		if (cfg_sort_type >= TIME_ASC && cfg_sort_type <= TRACKS_DESC)
			add_callback();
	}

	void on_items_removed(t_size p_playlist,const bit_array & p_mask,t_size p_old_count,t_size p_new_count)
	{
		if (cfg_sort_type >= TIME_ASC && cfg_sort_type <= TRACKS_DESC)
			add_callback();
	}

	void on_playlist_created(t_size p_index,const char * p_name,t_size p_name_len)
	{
		add_callback();
	}

	void on_playlist_renamed(t_size p_index,const char * p_new_name,t_size p_new_name_len)
	{
		if (cfg_sort_type == ASC || cfg_sort_type == DESC)
			add_callback();
	}


	void add_callback()
	{
		if (advconfig_autosort.get())
		{
			service_ptr_t<playlists_resort> plc = new service_impl_t<playlists_resort>();
			static_api_ptr_t<main_thread_callback_manager>()->add_callback(plc);
		}
	}
};


class foo_playlists_sort : public initquit
{
	virtual void on_init()
	{
		// Registering callbacks and sorting
		(new playlists_callback())->add_callback();
	}

	virtual void on_quit()
	{
		//nothing to do at all
	}
};

initquit_factory_t<foo_playlists_sort> foo_initquit;
