ScriptButton button;

/*
 */
int getClass()
{
	return button;
}

int hasSwitcherBehaviour()
{
	return 1;
}

/*
 */
void init(ObjScript script)
{
	button = new ScriptButton(script);
}

void shutdown()
{
	delete button;
	button = NULL;
}

/*
 */
void update()
{
	button.update();
}

/*
 */
void grabIt(int num_hand)
{
	button.press();
}
