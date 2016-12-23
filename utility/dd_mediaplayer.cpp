/******************************************************************************
*    Copyright (C), 2015 by HappyTown                                         *
*    FileName:    dd_mediaPlayer.cpp
*    Author:      WangYing	                                             *
*    Description:                                                             *
*    History:                                                                 *
*      <author>          <time>          <version>          <description>     *
*        Xzj        2016-12-22 11:24      V1.0.0                build         *
*                                                                             *
******************************************************************************/
#include <gst/gst.h>

#include "../dandan.h"

class ddMediaPlayerPrivate : public ddPrivateBase, public ddTimer::interface {
	DD_PUBLIC_DECLARE(ddMediaPlayer)
public:
	ddMediaPlayerPrivate () : m_pOwner(nil), m_playState(DDENM_PLAYSTATE_STOP), m_processTimer(this), m_busWatchId(0) {
		if ( !gst_is_initialized() ) {
			gst_init (nil, nil);
		}
		m_pPlaybin = gst_element_factory_make ("playbin", "dd_media_player");
//		m_pAutovideosink = gst_element_factory_make("ximagesink", "dd_avsink");
		m_pAutovideosink = gst_element_factory_make("autovideosink", "dd_avsink");
		if ( m_pPlaybin && m_pAutovideosink ) {
			g_object_set(m_pPlaybin, "video-sink", m_pAutovideosink, NULL);
			GstBus *pBus = gst_pipeline_get_bus(GST_PIPELINE(m_pPlaybin));
			if ( pBus ) {
				m_busWatchId = gst_bus_add_watch(pBus, onBusCallback, this);
				gst_object_unref(pBus);
			}
		}
	}

	~ddMediaPlayerPrivate() {
		if ( gst_is_initialized() ) {
			gst_deinit();
		}
		if ( m_pPlaybin ) {
			gst_element_set_state(m_pPlaybin, GST_STATE_NULL);
			gst_object_unref(GST_OBJECT(m_pPlaybin));
		}
		if ( m_pAutovideosink ) {
			gst_object_unref(GST_OBJECT(m_pAutovideosink));
		}
		if ( m_busWatchId ) {
			g_source_remove(m_busWatchId);
		}
	}
 
	static gboolean onBusCallback(GstBus* bus, GstMessage* msg, gpointer data) {
		if ( msg && data ) {
			ddMediaPlayerPrivate *pThis = (ddMediaPlayerPrivate*)data;
			switch ( GST_MESSAGE_TYPE(msg) ) {
				case GST_MESSAGE_EOS: pThis->handleEosMessage(msg); break;
				case GST_MESSAGE_ERROR: pThis->handleErrorMessage(msg); break;
				case GST_MESSAGE_WARNING: pThis->handleWarningMessage(msg); break;
				case GST_MESSAGE_TAG: pThis->handleTagMessage(msg); break;
				case GST_MESSAGE_STATE_CHANGED: pThis->handleStateChangedMessage(msg); break;
				default: break;
			}
			return yes;
		}
		return no;///< event source should be removed
	}

	ddVoid handleEosMessage(GstMessage* msg) {
		m_processTimer.killTimer();
		gst_element_set_state (m_pPlaybin, GST_STATE_NULL);
		m_playState = DDENM_PLAYSTATE_STOP;
		if ( m_pOwner ) {
			m_pOwner->onMediaPlayer_playEnd();
			m_pOwner->onMediaPlayer_playState(m_playState);
		}
	}

	ddVoid handleErrorMessage(GstMessage* msg) {
		m_processTimer.killTimer();
		gst_element_set_state (m_pPlaybin, GST_STATE_NULL);
		GError* err = nil;
	   	gchar *debug = nil;
		gst_message_parse_error(msg, &err, &debug);
		if ( debug ) {
			g_free(debug);
			debug = nil;
		}
		if ( err ) {
			if ( m_pOwner ) {
				m_pOwner->onMediaPlayer_playError(err->message);
			}
			g_error_free(err);
			err = nil;
		}
	}

	ddVoid handleWarningMessage(GstMessage* msg) {
		GError* err = nil;
	   	gchar* debug = nil;
		gst_message_parse_warning(msg, &err, &debug);
		if ( debug ) {
			g_free(debug);
			debug = nil;
		}
		if ( err ) {
			if ( m_pOwner ) {
				m_pOwner->onMediaPlayer_playWarning(err->message);
			}
			g_error_free(err);
			err = nil;
		}
	}

	ddVoid handleTagMessage(GstMessage* msg) {
		GstTagList *tags = NULL;
		gst_message_parse_tag (msg, &tags);
		gchar *title = NULL, *artist = NULL, *album= NULL;
		gst_tag_list_get_string(tags, GST_TAG_TITLE, &title);
		gst_tag_list_get_string(tags, GST_TAG_ARTIST, &artist);
		gst_tag_list_get_string(tags, GST_TAG_ALBUM, &album);
		printf("player_bus_callback,GST_MESSAGE_TAG,title:%s,artist:%s,album:%s\n", title, artist, album);
		g_free(title);
		g_free(artist);
		g_free(album);
		gst_tag_list_unref (tags);
/*		{
			GstTagList *tags = NULL;
			gst_message_parse_tag (msg, &tags);
			g_print ("player_bus_callback,Got tags from element %s:\n", GST_OBJECT_NAME (GST_MESSAGE_SRC(msg)));
			gst_tag_list_foreach (tags, print_one_tag, NULL);
		}*/
	}

	ddVoid handleStateChangedMessage(GstMessage* msg) {
		GstState oldstate, newstate, pending;
		gst_message_parse_state_changed(msg, &oldstate, &newstate, &pending);
		if ( oldstate != newstate ) {
			emMediaPlayState state = DDENM_PLAYSTATE_STOP;
			switch ( newstate ) {
				case GST_STATE_PLAYING: 
					state = DDENM_PLAYSTATE_PLAY;
					m_processTimer.setTimer(1000, yes);
				   	break;
				case GST_STATE_PAUSED:
				   	state = DDENM_PLAYSTATE_PAUSE;
					m_processTimer.killTimer();
				   	break;
				default:
					m_processTimer.killTimer();
				   	break;
			}
			if ( state != m_playState ) {
				m_playState = state;
				if ( m_pOwner ) {
					m_pOwner->onMediaPlayer_playState(state);
				}
			}
		}
	}

	virtual ddVoid onTimer(ddTimer* pTimer) {
		if ( pTimer == &m_processTimer ) {
			if ( DDENM_PLAYSTATE_PLAY == m_playState ) {
				gint64 pos = 0, dur = 0;
				if ( m_pOwner && gst_element_query_position(m_pPlaybin, GST_FORMAT_TIME, &pos)
						&& gst_element_query_duration(m_pPlaybin, GST_FORMAT_TIME, &dur) ) {
					m_pOwner->onMediaPlayer_process(pos/1000000000, dur/1000000000);
				}
			}
		}
	}

	ddVoid setOwner(ddMediaPlayer::interface* pOwner) {
		m_pOwner = pOwner;
	}

	ddVoid playFile(ddpCChar pFileName) {
		if ( m_pPlaybin && pFileName && strlen(pFileName) ) {
			gst_element_set_state (m_pPlaybin, GST_STATE_NULL);
			gchar* pUri = g_filename_to_uri(pFileName, nil, nil);
			if ( pUri ) {
				g_object_set(G_OBJECT(m_pPlaybin), "uri", pUri, NULL);
				gst_element_set_state(m_pPlaybin, GST_STATE_PLAYING);
				g_free(pUri);
				pUri = NULL;
			}
		}
	}

	ddVoid play() {
		if ( m_pPlaybin && DDENM_PLAYSTATE_PLAY != m_playState ) {
			gst_element_set_state(m_pPlaybin, GST_STATE_PLAYING);
		}
	}

	ddVoid pause() {
		if ( m_pPlaybin && DDENM_PLAYSTATE_PLAY == m_playState ) {
			gst_element_set_state(m_pPlaybin, GST_STATE_PAUSED);
		}
	}

	ddVoid seek(ddUInt32 pos) {
		if ( m_pPlaybin ) {
			gint64 duration = 0;
			gst_element_query_duration(m_pPlaybin, GST_FORMAT_TIME, &duration);
			if ( pos < duration/1000000000 ) {
				gst_element_seek (m_pPlaybin, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
						GST_SEEK_TYPE_SET, pos * 1000000000, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
			}
		}
	}

private:
	GstElement *m_pPlaybin;
	GstElement *m_pAutovideosink;
	ddMediaPlayer::interface *m_pOwner;
	emMediaPlayState m_playState;
	ddTimer m_processTimer;
	guint m_busWatchId;
};

///////////////////////////////////////////////////////////////////////////////
ddMediaPlayer::ddMediaPlayer(interface* pOwner)
{
	DD_D_NEW(ddMediaPlayerPrivate);
	setOwner(pOwner);
}

ddMediaPlayer::~ddMediaPlayer()
{
	DD_D_DELETE();
}

ddVoid ddMediaPlayer::setOwner(interface* pOwner)
{
	dPtr()->setOwner(pOwner);
}

ddVoid ddMediaPlayer::playFile(ddpCChar pFileName)
{
	dPtr()->playFile(pFileName);
}

ddVoid ddMediaPlayer::play()
{
	dPtr()->play();
}

ddVoid ddMediaPlayer::pause()
{
	dPtr()->pause();
}

ddVoid ddMediaPlayer::seek(ddUInt32 pos)
{
	dPtr()->seek(pos);
}
