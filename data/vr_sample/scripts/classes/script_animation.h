#ifndef __SCRIPT_ANIMATION_H_INCLUDED__
#define __SCRIPT_ANIMATION_H_INCLUDED__

class ScriptAnimation
{
private:
	ObjScript script;
	ObjectMeshSkinned mesh_skinned;

	string animations[0];

private:
	int valid_parameter(PropertyParameter parameter)
	{
		if (parameter == NULL)
			return 0;

		if (parameter.getType() != PROPERTY_PARAMETER_ARRAY)
			return 0;

		if (parameter.getArrayType() != PROPERTY_PARAMETER_FILE)
			return 0;

		if (parameter.getArrayDim() != 1)
			return 0;

		return 1;
	}

public:
	ScriptAnimation(ObjScript s)
	{
		script = s;

		Node node = script.getNode();

		Node hierarchy[0] = ();
		node.getHierarchy(hierarchy);

		forloop(int i = 0; hierarchy.size())
		{
			if (hierarchy[i].getType() == NODE_OBJECT_MESH_SKINNED)
			{
				mesh_skinned = node_cast(hierarchy[i]);
				break;
			}
		}

		int index = node.findProperty("script_animation_data");
		if (index != -1)
		{
			Property property = node.getProperty(index);
			PropertyParameter animation_parameters = property.getParameterPtr("animations");
			if (valid_parameter(animation_parameters))
			{
				forloop(int i = 0; animation_parameters.getArraySize())
				{
					PropertyParameter path = animation_parameters.getChild(i);
					animations.append(path.getValueFile());
				}
			}
		}
	}

	void setSpeed(float value) { mesh_skinned.setSpeed(value); }
	float getSpeed() { return mesh_skinned.getSpeed(); }

	void setLooped(int value) { mesh_skinned.setLoop(value); }
	int isLooped() { return mesh_skinned.getLoop(); }

	int isPlaying() { return mesh_skinned.isPlaying(); }

	int getNumAnimations() { return animations.size(); }
	void setAnimation(int index)
	{
		if (index < 0 || index >= animations.size())
			return;

		mesh_skinned.setAnimation(0, animations[index]);
	}

	void play(int index)
	{
		setAnimation(index);
		play();
	}

	void play()
	{
		mesh_skinned.setFrame(0, 0.0f);
		mesh_skinned.play();
	}

	void stop()
	{
		mesh_skinned.stop();
		mesh_skinned.setFrame(0, 0.0f);
	}

	void pause() { mesh_skinned.stop(); }
	void unpause() { mesh_skinned.play(); }
};

#endif /* __SCRIPT_ANIMATION_H_INCLUDED__ */
