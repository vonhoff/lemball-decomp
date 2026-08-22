#ifndef LEMBALL_AI_MESSAGES_REQUESTCANCELMESS_H
#define LEMBALL_AI_MESSAGES_REQUESTCANCELMESS_H

#include "../../Common.h"
#include "GameObjectMess.h" // complete type

class RequestCancelMess : public GameObjectMess {
public:
	RequestCancelMess();
	~RequestCancelMess();
};

#endif
