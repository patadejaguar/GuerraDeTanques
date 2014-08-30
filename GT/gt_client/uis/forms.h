#ifndef FORMS_H
#define FORMS_H

#include "formprincipal.h"

#include "formreplayselect.h"
#include "formreplayview.h"

#include "formconfiguration.h"
#include "formcontrols.h"
#include "formsound.h"
#include "formnetwork.h"

#include "formhelp.h"
#include "formcredits.h"

#include "formconnectedtoserver.h"
#include "formdialogmessage.h"
#include "formcreateuseraccount.h"
#include "formloadavatar.h"
#include "formedituseraccount.h"
#include "formchat.h"
#include "formcreategame.h"
#include "formcreatedgame.h"
#include "formjoingame.h"
#include "formtankselector.h"
#include "formgame.h"
#include "forminitgame.h"
#include "formresults.h"

class Forms
{
    public:

        static Forms*   instance();

        Form*               _form_principal;

        Form*               _form_replays_select;

        FormReplayView*     _form_replays_view;

        Form*               _form_configuration;

        Form*               _form_controls;

        Form*               _form_sound;

        Form*               _form_network;

        Form*               _form_help;

        Form*               _form_credits;

        Form*               _form_connected_to_server;

        Form*               _form_create_user_account;

        Form*               _form_load_avatar;

        Form*               _form_edit_user_account;

        Form*               _form_chat;

        Form*               _form_create_game;

        Form*               _form_created_game;

        Form*               _form_join_game;

        Form*               _form_tank_selector;

        Form*               _form_game;

        Form*               _form_message;

        FormInitGame*       _form_init_game;

        FormResults*        _form_result;

    private:

        Forms();

        static Forms*       _instance;
};

#endif // FORMS_H
