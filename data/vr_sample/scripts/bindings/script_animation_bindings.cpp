ScriptAnimation animation;

/*
 */
int getClass()
{
	return animation;
}

/*
 */
void init(ObjScript script)
{
	animation = new ScriptAnimation(script);
}

void shutdown()
{
	delete animation;
	animation = NULL;
}
