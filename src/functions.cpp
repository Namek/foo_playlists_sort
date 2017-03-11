#include "main.h"
#include "config.h"


struct PlaylistName
{
	pfc::string name;
	t_size num;
	double time;
	t_size tracks;
};


void SortPlaylists(SORT_TYPE sort_type)
{
	static_api_ptr_t<playlist_manager> pm;

	pfc::string_formatter playlist_name;
	t_size playlists_count;
	pfc::list_t<PlaylistName> playlists;
	

	// get playlists count
	if (!(playlists_count = pm->get_playlist_count()))
		return;

	// get all playlists' names
	for (t_size i = 0; i < playlists_count; ++i)
	{
		PlaylistName playlist;

		pm->playlist_get_name(i, playlist_name);
		playlist.name = pfc::string(playlist_name.get_ptr());
		if (!advconfig_linux_like)
			playlist.name = playlist.name.toUpper();
		playlist.num = i;


		// count time
		if (sort_type == TIME_ASC || sort_type == TIME_DESC)
		{
			pfc::list_t<metadb_handle_ptr> playlist_tracks;

			pm->playlist_get_all_items(i, playlist_tracks);
			t_size playlist_tracks_count = playlist_tracks.get_count();

			for (t_size j = 0; j < playlist_tracks_count; ++j)
			{
				metadb_handle_ptr& track = playlist_tracks.get_item(j);
				playlist.time = track.get_ptr()->get_length();
			}
		}

		//count number of tracks
		if (sort_type == TRACKS_ASC || sort_type == TRACKS_DESC)
			playlist.tracks = pm->playlist_get_item_count(i);


		playlists.add_item(playlist);
	}

	// sort playlists
	for (t_size i = 0; i < playlists_count-1; ++i)
	{
		int boundary_index = i;

		for (t_size j = i+1; j < playlists_count; ++j)
		{
			bool cmp;
			switch (sort_type)
			{
				case ASC:			cmp = playlists[j].name < playlists[boundary_index].name; break;
				case DESC:			cmp = playlists[j].name > playlists[boundary_index].name; break;
				case TIME_ASC:		cmp = playlists[j].time < playlists[boundary_index].time; break;
				case TIME_DESC:		cmp = playlists[j].time > playlists[boundary_index].time; break;
				case TRACKS_ASC:	cmp = playlists[j].tracks < playlists[boundary_index].tracks; break;
				case TRACKS_DESC:	cmp = playlists[j].tracks > playlists[boundary_index].tracks; break;
			}

			if (cmp) boundary_index = j;
		}

		// swap items
		if (i != boundary_index)
			playlists.swap_items(i, boundary_index);
	}
	

	// create order indices array for reorder
	t_size* order_indices = new t_size[playlists_count];

	for (t_size i = 0; i < playlists_count; ++i)
		order_indices[i] = playlists[i].num;


	// save the sort result
	pm->reorder(order_indices, playlists_count);

	// clear the data
	delete[] order_indices;
	playlists.remove_all();
}
