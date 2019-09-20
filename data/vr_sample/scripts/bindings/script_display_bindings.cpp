ScriptDisplay display;

/*
 */
int getClass()
{
	return display;
}

/*
 */
void init(ObjScript script)
{
	display = new ScriptDisplay(script);
}

void shutdown()
{
	delete display;
	display = NULL;
}
