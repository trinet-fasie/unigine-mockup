#ifndef __APP_SYSTEM_LOGIC_H__
#define __APP_SYSTEM_LOGIC_H__

#include <UnigineLogic.h>

// System logic, it exists during the application life cycle
class AppSystemLogic : public Unigine::SystemLogic
{
public:
	AppSystemLogic() {}
	virtual ~AppSystemLogic() {}
	
	int init() override;
	int shutdown() override;
};

#endif // __APP_SYSTEM_LOGIC_H__
