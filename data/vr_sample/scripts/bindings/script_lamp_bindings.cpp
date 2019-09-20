ScriptLamp lamp;

/*
 */
int getClass()
{
	return lamp;
}

/*
 */
void init(ObjScript script)
{
	lamp = new ScriptLamp(script);
}

void shutdown()
{
	delete lamp;
	lamp = NULL;
}
