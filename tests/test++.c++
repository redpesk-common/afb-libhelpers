// Copyright (C) 2015-2021 IoT.bzh Company
// Author: José Bollo <jose.bollo@iot.bzh>
//
// SPDX-License-Identifier: LGPL-3.0-only

#include <string>
#include <cstring>
#include <utility>

#define AFB_BINDING_VERSION 4
#include <afb/afb-binding>

#include "../afb-data-utils"
#include "../json.hpp"

using nlohmann::json;

afb::event event_login, event_logout;

void reply_error(afb::req req, const char *text)
{
	afb::dataset<1> a;
	a[0] = afb_data_string(text);
	req.reply(-1, a);
}

void push(afb::event &evt, const std::string &value)
{
	afb::dataset<1> a;
	a[0] = afb_data_string(value.c_str());
	evt.push(a);
}

class session
{
private:
	std::string user_;
public:
	session(const std::string &user) : user_(user) {}
	~session() {}
	operator const char *() const { return user_.c_str(); }
	operator std::string() const { return std::string(user_); }
};

void login(afb::req req, afb::received_data params)
{
	json args, user, passwd;
	session *session;

	try {
		args = params;
		user = args["user"];
		if (user.is_null())
			throw json::other_error::create(0,"no user",args);
		passwd = args["password"];
		if (passwd.is_null())
			throw json::other_error::create(0,"no password",args);
		session = req.context<class session>().get();
		if (session != nullptr) {
			AFB_REQ_ERROR(req, "login, bad state, logout first");
			reply_error(req, "bad-state");
		}
		else if (passwd != "please") {
			AFB_REQ_ERROR(req, "login, unauthorized");
			reply_error(req, "unauthorized");
		}
		else {
			session = new (class session)(user);
			AFB_REQ_NOTICE(req, "login user: %s", (const char*)session);
			req.context<class session>().set(session);
			req.session_set_LOA(1);
			req.reply();
			push(event_login, *session);
		}
	}
	catch (json::exception &e) {
		AFB_REQ_ERROR(req, "login, bad request");
		reply_error(req, "bad-request");
	}
	catch (...) {
		AFB_REQ_ERROR(req, "internal-error");
		reply_error(req, "internal error");
	}
}

void action(afb::req req, afb::received_data params)
{
	json args, sub;

	session *session = req.context<class session>().get();

	try {
		args = params;
		sub = args["subscribe"];
		if (!sub.is_null()) {
			if (sub) {
				AFB_REQ_NOTICE(req, "user %s subscribes to events", (const char*)session);
				req.subscribe(event_login);
				req.subscribe(event_logout);
			}
			else {
				AFB_REQ_NOTICE(req, "user %s unsubscribes to events", (const char*)session);
				req.unsubscribe(event_login);
				req.unsubscribe(event_logout);
			}
		}
	}
	catch (...) {
	}
	req.reply();
}

void logout(afb::req req, afb::received_data params)
{
	session *session = req.context<class session>().get();

	AFB_REQ_NOTICE(req, "login user %s out", (const char*)session);
	push(event_logout, *session);
	req.session_set_LOA(0);
	req.context<class session>().clear();
	req.reply();
}

int mainctl(afb_api_t api, afb_ctlid_t ctlid, afb_ctlarg_t ctlarg, void *userdata)
{
	afb::api a(api);

	if (ctlid == afb_ctlid_Init) {
		AFB_NOTICE("init");
		event_login = a.new_event("login");
		event_logout = a.new_event("logout");
		if (!event_login || !event_login) {
			AFB_ERROR("Can't create events");
			return -1;
		}
	}
	return 0;
}

const afb_verb_t verbs[] = {
	afb::verb<login>("login", "log in the system"),
	afb::verb<action>("action", "perform an action", AFB_SESSION_LOA_1),
	afb::verb<logout>("logout", "log out the system", AFB_SESSION_LOA_1),
	afb::verbend()
};

const afb_binding_t afbBindingExport = afb::binding("tuto-3", verbs, "third tutorial: C++", mainctl);
