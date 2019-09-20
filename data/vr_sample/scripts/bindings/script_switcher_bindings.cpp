ScriptSwitcher switcher;

/*
 */
int getClass()
{
	return switcher;
}

int hasSwitcherBehaviour()
{
	return 1;
}

/*
 */
void init(ObjScript script)
{
	switcher = new ScriptSwitcher(script);
}

void shutdown()
{
	delete switcher;
	switcher = NULL;
}

/*
 */
void update()
{
	switcher.update();
}

/*
 */
void grabIt(int num_hand)
{
	switcher.toggle();
}
