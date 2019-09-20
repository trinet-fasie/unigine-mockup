/* Copyright (C) 2005-2019, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.9 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */


#ifndef __UNIGINE_UNDERSCORE_H__
#define __UNIGINE_UNDERSCORE_H__

/* Useful functional programming helpers, inspired by Underscore.js
 */
namespace Unigine
{
class Underscore
{
public:
	void each(int collection[], int action)
	{
		assert(is_function(action, 1) || is_function(action, 2) || is_function(action, 3));

		if (is_function(action, 1))
			forloop(int i = 0; collection.size())
				call(action, collection[collection.key(i)]);
		if (is_function(action, 2))
			forloop(int i = 0; collection.size())
				call(action, collection[collection.key(i)], collection.key(i));
		if (is_function(action, 3))
			forloop(int i = 0; collection.size())
				call(action, collection[collection.key(i)], collection.key(i), collection.id());
	}

	void map(int result[], int collection[], int mapper = 0)
	{
		if (mapper == 0)
			mapper = [](int x) { return x; };

		assert(is_function(mapper, 1) || is_function(mapper, 2) || is_function(mapper, 3));

		result.clear();

		if (is_vector(result))
		{
			if (is_function(mapper, 1))
				forloop(int i = 0; collection.size())
					result.append(call(mapper, collection[collection.key(i)]));
			if (is_function(mapper, 2))
				forloop(int i = 0; collection.size())
					result.append(call(mapper, collection[collection.key(i)], collection.key(i)));
			if (is_function(mapper, 3))
				forloop(int i = 0; collection.size())
					result.append(call(mapper, collection[collection.key(i)], collection.key(i), collection.id()));
		} else if (is_map(result))
		{
			if (is_function(mapper, 1))
				forloop(int i = 0; collection.size())
					result.append(collection.key(i), call(mapper, collection[collection.key(i)]));
			if (is_function(mapper, 2))
				forloop(int i = 0; collection.size())
					result.append(collection.key(i), call(mapper, collection[collection.key(i)], collection.key(i)));
			if (is_function(mapper, 3))
				forloop(int i = 0; collection.size())
					result.append(collection.key(i), call(mapper, collection[collection.key(i)], collection.key(i), collection.id()));
		}
	}

	void map(int result[], int first[], int second[], int mapper)
	{
		assert(is_function(mapper, 2));

		result.clear();

		int size = ::min(first.size(), second.size());

		if (is_vector(result))
			forloop(int i = 0; size)
				result.append(call(mapper, first[first.key(i)], second[second.key(i)]));
		if (is_map(result))
			forloop(int i = 0; size)
				result.append(i, call(mapper, first[first.key(i)], second[second.key(i)]));
	}

	void hash(int result[], int collection[], int mapper = 0)
	{
		if (mapper == 0)
			mapper = [](int x) { return x; };

		assert(is_function(mapper, 1) || is_function(mapper, 2) || is_function(mapper, 3));

		result.clear();

		if (is_function(mapper, 1))
			forloop(int i = 0; collection.size())
				result.append(call(mapper, collection[collection.key(i)]));
		if (is_function(mapper, 2))
			forloop(int i = 0; collection.size())
				result.append(call(mapper, collection[collection.key(i)], collection.key(i)));
		if (is_function(mapper, 3))
			forloop(int i = 0; collection.size())
				result.append(call(mapper, collection[collection.key(i)], collection.key(i), collection.id()));
	}

	void hash(int result[], int first[], int second[], int mapper)
	{
		assert(is_function(mapper, 2));

		result.clear();

		int size = ::min(first.size(), second.size());

		forloop(int i = 0; size) result.append(call(mapper, first[first.key(i)], second[second.key(i)]));
	}

	int fold(int collection[], int func)
	{
		return foldLeft(collection, func);
	}

	int foldLeft(int collection[], int func)
	{
		assert(is_function(func, 2) || is_function(func, 3));
		assert(collection.size() > 0);

		int memo = collection[collection.key(0)];

		if (is_function(func, 2))
			forloop(int i = 1; collection.size())
				memo = call(func, memo, collection[collection.key(i)]);
		if (is_function(func, 3))
			forloop(int i = 1; collection.size())
				memo = call(func, memo, collection[collection.key(i)], collection.id());

		return memo;
	}

	int foldRight(int collection[], int func)
	{
		assert(is_function(func, 2) || is_function(func, 3));
		assert(collection.size() > 0);

		int memo = collection[collection.key(collection.size() - 1)];

		if (is_function(func, 2))
			for (int i = collection.size() - 2; i >= 0; i--)
				memo = call(func, memo, collection[collection.key(i)]);
		if (is_function(func, 3))
			for (int i = collection.size() - 2; i >= 0; i--)
				memo = call(func, memo, collection[collection.key(i)], collection.id());

		return memo;
	}

	int reduce(int collection[], int memo, int func)
	{
		return reduceLeft(collection, memo, func);
	}

	int reduceLeft(int collection[], int memo, int func)
	{
		assert(is_function(func, 1) || is_function(func, 2) || is_function(func, 3) || is_function(func, 4));

		if (is_function(func, 1))
			forloop(int i = 0; collection.size())
				memo = call(func, memo);
		if (is_function(func, 2))
			forloop(int i = 0; collection.size())
				memo = call(func, memo, collection[collection.key(i)]);
		if (is_function(func, 3))
			forloop(int i = 0; collection.size())
				memo = call(func, memo, collection[collection.key(i)], collection.key(i));
		if (is_function(func, 4))
			forloop(int i = 0; collection.size())
				memo = call(func, memo, collection[collection.key(i)], collection.key(i), collection.id());

		return memo;
	}

	int reduceRight(int collection[], int memo, int func)
	{
		assert(is_function(func, 1) || is_function(func, 2) || is_function(func, 3) || is_function(func, 4));

		if (is_function(func, 1))
			for (int i = collection.size() - 1; i >= 0; i--)
				memo = call(func, memo);
		if (is_function(func, 2))
			for (int i = collection.size() - 1; i >= 0; i--)
				memo = call(func, memo, collection[collection.key(i)]);
		if (is_function(func, 3))
			for (int i = collection.size() - 1; i >= 0; i--)
				memo = call(func, memo, collection[collection.key(i)], collection.key(i));
		if (is_function(func, 4))
			for (int i = collection.size() - 1; i >= 0; i--)
				memo = call(func, memo, collection[collection.key(i)], collection.key(i), collection.id());

		return memo;
	}

	int filter(int collection[], int predicate = 0)
	{
		if (predicate == 0)
			predicate = [](int x) { return x; };

		assert(is_function(predicate, 1) || is_function(predicate, 2) || is_function(predicate, 3));

		int old_size = collection.size();

		if (is_function(predicate, 1))
		{
			for (int i = 0; i < collection.size(); i++)
			{
				int key = collection.key(i);
				if (!call(predicate, collection[key]))
				{
					collection.remove(key);
					i--;
				}
			}
		}
		if (is_function(predicate, 2))
		{
			if (is_vector(collection))
			{
				for (int i = 0, key = 0; i < collection.size(); i++, key++)
				{
					if (!call(predicate, collection[i], key))
					{
						collection.remove(i);
						i--;
					}
				}
			}
			if (is_map(collection))
			{
				for (int i = 0; i < collection.size(); i++)
				{
					int key = collection.key(i);
					if (!call(predicate, collection[key], key))
					{
						collection.remove(key);
						i--;
					}
				}
			}
		}
		if (is_function(predicate, 3))
		{
			if (is_vector(collection))
			{
				for (int i = 0, key = 0; i < collection.size(); i++, key++)
				{
					if (!call(predicate, collection[i], key, collection.id()))
					{
						collection.remove(i);
						i--;
					}
				}
			}
			if (is_map(collection))
			{
				for (int i = 0; i < collection.size(); i++)
				{
					int key = collection.key(i);
					if (!call(predicate, collection[key], key, collection.id()))
					{
						collection.remove(key);
						i--;
					}
				}
			}
		}

		int removed = old_size - collection.size();

		return removed;
	}

	void filter(int result[], int collection[], int predicate = 0)
	{
		if (predicate == 0)
			predicate = [](int x) { return x; };

		assert(is_function(predicate, 1) || is_function(predicate, 2) || is_function(predicate, 3));

		result.clear();

		if (is_vector(result))
		{
			if (is_function(predicate, 1))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (call(predicate, item))
						result.append(item);
				}
			if (is_function(predicate, 2))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (call(predicate, item, collection.key(i)))
						result.append(item);
				}
			if (is_function(predicate, 3))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (call(predicate, item, collection.key(i), collection.id()))
						result.append(item);
				}
		} else if (is_map(result))
		{
			if (is_function(predicate, 1))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (call(predicate, item))
						result.append(collection.key(i), item);
				}
			if (is_function(predicate, 2))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (call(predicate, item, collection.key(i)))
						result.append(collection.key(i), item);
				}
			if (is_function(predicate, 3))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (call(predicate, item, collection.key(i), collection.id()))
						result.append(collection.key(i), item);
				}
		}
	}

	int reject(int collection[], int predicate = 0)
	{
		if (predicate == 0)
			predicate = [](int x) { return x; };

		assert(is_function(predicate, 1) || is_function(predicate, 2) || is_function(predicate, 3));

		int old_size = collection.size();

		if (is_function(predicate, 1))
		{
			for (int i = 0; i < collection.size(); i++)
			{
				int key = collection.key(i);
				if (call(predicate, collection[key]))
				{
					collection.remove(key);
					i--;
				}
			}
		}
		if (is_function(predicate, 2))
		{
			if (is_vector(collection))
			{
				for (int i = 0, key = 0; i < collection.size(); i++, key++)
				{
					if (call(predicate, collection[i], key))
					{
						collection.remove(i);
						i--;
					}
				}
			}
			if (is_map(collection))
			{
				for (int i = 0; i < collection.size(); i++)
				{
					int key = collection.key(i);
					if (call(predicate, collection[key], key))
					{
						collection.remove(key);
						i--;
					}
				}
			}
		}
		if (is_function(predicate, 3))
		{
			if (is_vector(collection))
			{
				for (int i = 0, key = 0; i < collection.size(); i++, key++)
				{
					if (call(predicate, collection[i], key, collection.id()))
					{
						collection.remove(i);
						i--;
					}
				}
			}
			if (is_map(collection))
			{
				for (int i = 0; i < collection.size(); i++)
				{
					int key = collection.key(i);
					if (call(predicate, collection[key], key, collection.id()))
					{
						collection.remove(key);
						i--;
					}
				}
			}
		}

		int removed = old_size - collection.size();

		return removed;
	}

	void reject(int result[], int collection[], int predicate = 0)
	{
		if (predicate == 0)
			predicate = [](int x) { return x; };

		assert(is_function(predicate, 1) || is_function(predicate, 2) || is_function(predicate, 3));

		result.clear();

		if (is_vector(result))
		{
			if (is_function(predicate, 1))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (!call(predicate, item))
						result.append(item);
				}
			if (is_function(predicate, 2))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (!call(predicate, item, collection.key(i)))
						result.append(item);
				}
			if (is_function(predicate, 3))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (!call(predicate, item, collection.key(i), collection.id()))
						result.append(item);
				}
		} else if (is_map(result))
		{
			if (is_function(predicate, 1))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (!call(predicate, item))
						result.append(collection.key(i), item);
				}
			if (is_function(predicate, 2))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (!call(predicate, item, collection.key(i)))
						result.append(collection.key(i), item);
				}
			if (is_function(predicate, 3))
				forloop(int i = 0; collection.size())
				{
					int item = collection[collection.key(i)];
					if (!call(predicate, item, collection.key(i), collection.id()))
						result.append(collection.key(i), item);
				}
		}
	}

	int find(int collection[], int predicate = 0, int undefined = -1)
	{
		if (predicate == 0)
			predicate = [](int x) { return x; };

		assert(is_function(predicate, 1) || is_function(predicate, 2) || is_function(predicate, 3));

		if (is_function(predicate, 1))
			forloop(int i = 0; collection.size())
			{
				int item = collection[collection.key(i)];
				if (call(predicate, item))
					return item;
			}
		if (is_function(predicate, 2))
			forloop(int i = 0; collection.size())
			{
				int item = collection[collection.key(i)];
				if (call(predicate, item, collection.key(i)))
					return item;
			}
		if (is_function(predicate, 3))
			forloop(int i = 0; collection.size())
			{
				int item = collection[collection.key(i)];
				if (call(predicate, item, collection.key(i), collection.id()))
					return item;
			}

		return undefined;
	}

	int findKey(int collection[], int predicate = 0, int undefined = -1)
	{
		if (predicate == 0)
			predicate = [](int x) { return x; };

		assert(is_function(predicate, 1) || is_function(predicate, 2) || is_function(predicate, 3));

		if (is_function(predicate, 1))
			forloop(int i = 0; collection.size()) if (call(predicate, collection[collection.key(i)])) return collection.key(i);
		if (is_function(predicate, 2))
			forloop(int i = 0; collection.size()) if (call(predicate, collection[collection.key(i)], collection.key(i))) return collection.key(i);
		if (is_function(predicate, 3))
			forloop(int i = 0; collection.size()) if (call(predicate, collection[collection.key(i)], collection.key(i), collection.id())) return collection.key(i);

		return undefined;
	}

	int any(int collection[], int predicate = 0)
	{
		if (predicate == 0)
			predicate = [](int x) { return x; };

		assert(is_function(predicate, 1) || is_function(predicate, 2) || is_function(predicate, 3));

		if (is_function(predicate, 1))
			forloop(int i = 0; collection.size()) if (call(predicate, collection[collection.key(i)])) return true;
		if (is_function(predicate, 2))
			forloop(int i = 0; collection.size()) if (call(predicate, collection[collection.key(i)], collection.key(i))) return true;
		if (is_function(predicate, 3))
			forloop(int i = 0; collection.size()) if (call(predicate, collection[collection.key(i)], collection.key(i), collection.id())) return true;

		return false;
	}

	int every(int collection[], int predicate = 0)
	{
		if (predicate == 0)
			predicate = [](int x) { return x; };

		assert(is_function(predicate, 1) || is_function(predicate, 2) || is_function(predicate, 3));

		if (is_function(predicate, 1))
			forloop(int i = 0; collection.size()) if (!call(predicate, collection[collection.key(i)])) return false;
		if (is_function(predicate, 2))
			forloop(int i = 0; collection.size()) if (!call(predicate, collection[collection.key(i)], collection.key(i))) return false;
		if (is_function(predicate, 3))
			forloop(int i = 0; collection.size()) if (!call(predicate, collection[collection.key(i)], collection.key(i), collection.id())) return false;

		return true;
	}

	void reverse(int array[])
	{
		assert(is_vector(array));

		int left = 0;
		int right = array.size() - 1;

		while (left < right)
		{
			array.swap(left, right);
			left++;
			right--;
		}
	}

	void reverse(int result[], int array[])
	{
		assert(is_vector(result));
		assert(is_vector(array));

		result.copy(array);
		reverse(result);
	}

	int contains(int collection[], int x)
	{
		foreach (int item; collection)
		{
			if (item == x)
				return true;
		}
		return false;
	}

	int remove(int collection[], int x)
	{
		int old_size = collection.size();

		for (int i = 0; i < collection.size(); i++)
		{
			int key = collection.key(i);
			if (collection[key] == x)
			{
				collection.remove(key);
				i--;
			}
		}

		int removed = old_size - collection.size();

		return removed;
	}

	void remove(int result[], int collection[], int x)
	{
		result.clear();

		if (is_vector(result))
		{
			foreach (int item; collection)
			{
				if (item != x)
					result.append(item);
			}
		}
		if (is_map(result))
		{
			foreachkey(int key; collection)
			{
				int item = collection[key];
				if (item != x)
					result.append(key, item);
			}
		}
	}

	void test()
	{
		test_each();
		test_map();
		test_multi_map();
		test_hash();
		test_multi_hash();
		test_fold();
		test_reduce();
		test_filter();
		test_reject();
		test_find();
		test_find_key();
		test_any();
		test_every();
		test_reverse();
		test_contains();
		test_remove();
	}

private:
#define ensure(EXP)                                                                               \
	{                                                                                             \
		if (EXP)                                                                                  \
		{                                                                                         \
		} else                                                                                    \
		{                                                                                         \
			log.error("%s:%d: %s: assertion failed: '%s'\n", __FILE__, __LINE__, __FUNC__, #EXP); \
		}                                                                                         \
	}

	void test_each()
	{
		int count = 0;
		int id;
		int vector[0] = ();
		int map[] = (
			:);
		int keys[0] = ();
		int result[0] = ();

		// empty vector
		count = 0;
		vector = ();
		each(vector, [](int x) { count++; });
		ensure(count == 0);

		// vector value
		vector = (1, 2, 3);
		result = ();
		each(vector, [](int x) { result.append(x); });
		ensure(result.compare(vector));

		// vector value+key
		vector = (1, 2, 3);
		result = ();
		keys = ();
		each(vector, [](int x, int key) { result.append(x); keys.append(key); });
		ensure(result.compare(vector));
		ensure(keys.compare((0, 1, 2)));

		// vector value+key+collection
		count = 0;
		vector = (1, 2, 3);
		result = ();
		keys = ();
		each(vector, [](int x, int key, int xs[]) { result.append(x); keys.append(key); count += xs.size(); id = xs.id(); });
		ensure(result.compare(vector));
		ensure(keys.compare((0, 1, 2)));
		ensure(count == 9);
		ensure(id == vector.id());

		// empty map
		count = 0;
		map = (
			:);
		each(map, [](int x) { count++; });
		ensure(count == 0);

		// map value
		map = ("a" : 1, "b" : 2, "c" : 3);
		result = ();
		each(map, [](int x) { result.append(x); });
		ensure(result.compare((1, 2, 3)));

		// map value+key
		map = ("a" : 1, "b" : 2, "c" : 3);
		result = ();
		keys = ();
		each(map, [](int x, int key) { result.append(x); keys.append(key); });
		ensure(result.compare((1, 2, 3)));
		ensure(keys.compare(("a", "b", "c")));

		// map value+key+collection
		count = 0;
		map = ("a" : 1, "b" : 2, "c" : 3);
		result = ();
		keys = ();
		each(map, [](int x, int key, int xs[]) { result.append(x); keys.append(key); count += xs.size(); id = xs.id(); });
		ensure(result.compare((1, 2, 3)));
		ensure(keys.compare(("a", "b", "c")));
		ensure(count == 9);
		ensure(id == map.id());
	}

	void test_map()
	{
		int source_vector[0] = ();
		string result_vector[0] = ();
		int source_map[] = (
			:);
		string result_map[] = (
			:);

		// empty vector
		source_vector = ();
		result_vector = ("a", "b", "c");
		map(result_vector, source_vector);
		ensure(result_vector.size() == 0);

		// vector->vector value
		source_vector = (1, 2, 3);
		result_vector = ("a", "b", "c");
		map(result_vector, source_vector, [](int x) { return string(x); });
		ensure(result_vector.compare(("1", "2", "3")));

		// vector->map value
		source_vector = (1, 2, 3);
		result_map = (4
					  : "a", 5
					  : "b", 6
					  : "c");
		map(result_map, source_vector, [](int x) { return string(x); });
		ensure(result_map.compare((0
								   : "1", 1
								   : "2", 2
								   : "3")));

		// vector->map value+key
		source_vector = (1, 2, 3);
		result_map = (4
					  : "a", 5
					  : "b", 6
					  : "c");
		map(result_map, source_vector, [](int x, int i) { return string(i + x); });
		ensure(result_map.compare((0
								   : "1", 1
								   : "3", 2
								   : "5")));

		// map->vector value
		source_map = ("a" : 1, "b" : 2, "c" : 3);
		result_vector = ("a", "b", "c");
		map(result_vector, source_map, [](int x) { return string(x); });
		ensure(result_vector.compare(("1", "2", "3")));

		// map->vector value+key
		source_map = ("a" : 1, "b" : 2, "c" : 3);
		result_vector = ("a", "b", "c");
		map(result_vector, source_map, [](int x, string k) { return k + string(x); });
		ensure(result_vector.compare(("a1", "b2", "c3")));

		// map->map value
		source_map = ("a" : 1, "b" : 2, "c" : 3);
		result_map = (4
					  : "a", 5
					  : "b", 6
					  : "c");
		map(result_map, source_map, [](int x) { return string(x); });
		ensure(result_map.compare(("a"
								   : "1", "b"
								   : "2", "c"
								   : "3")));

		// map->map value+key
		source_map = ("a" : 1, "b" : 2, "c" : 3);
		result_map = (4
					  : "a", 5
					  : "b", 6
					  : "c");
		map(result_map, source_map, [](int x, string k) { return k + string(x); });
		ensure(result_map.compare(("a"
								   : "a1", "b"
								   : "b2", "c"
								   : "c3")));
	}

	void test_multi_map()
	{
		string result_vector[0] = ();
		string result_map[] = (
			:);

		// empty first
		result_vector = ("a", "b", "c");
		result_map = (4
					  : "a", 5
					  : "b", 6
					  : "c");
		map(result_vector, (), ("q", "w", "e", "r"), [](int x, string y) { return "" });
		map(result_map, (), ("q", "w", "e", "r"), [](int x, string y) { return "" });
		ensure(result_vector.size() == 0);
		ensure(result_map.size() == 0);

		// empty second
		result_vector = ("a", "b", "c");
		result_map = (4
					  : "a", 5
					  : "b", 6
					  : "c");
		map(result_vector, (1, 2, 3), (), [](int x, string y) { return "" });
		map(result_map, (1, 2, 3), (), [](int x, string y) { return "" });
		ensure(result_vector.size() == 0);
		ensure(result_map.size() == 0);

		// vector+vector -> vector
		result_vector = ("a", "b", "c");
		map(result_vector, (1, 2, 3), ("q", "w", "e", "r"), [](int x, string y) { return string(x) + y; });
		ensure(result_vector.compare(("1q", "2w", "3e")));

		// map+vector -> vector
		result_vector = ("a", "b", "c");
		map(result_vector, (1.0f : 1, 2.0f : 2, 3.0f : 3), ("q", "w"), [](int x, string y) { return string(x) + y; });
		ensure(result_vector.compare(("1q", "2w")));

		// vector+map -> map
		result_map = (4
					  : "a", 5
					  : "b", 6
					  : "c");
		map(result_map, (1, 2, 3), (1.0f
									   : "q", 2.0f
									   : "w", 3.0f
									   : "e", 4.0f
									   : "r"),
			[](int x, string y) { return string(x) + y; });
		ensure(result_map.compare((0
								   : "1q", 1
								   : "2w", 2
								   : "3e")));
	}

	void test_hash()
	{
		int source_vector[0] = ();
		string result_vector[0] = ();
		int source_map[] = (
			:);
		int result_map[] = (
			:);

		// empty vector
		source_vector = ();
		result_vector = ("a", "b", "c");
		hash(result_vector, source_vector);
		ensure(result_vector.size() == 0);

		// vector->vector value
		source_vector = (1, 2, 3);
		result_vector = ("a", "b", "c");
		hash(result_vector, source_vector, [](int x) { return string(x); });
		ensure(result_vector.compare(("1", "2", "3")));

		// vector->map value
		source_vector = (1, 2, 3);
		result_map = ("z" : 4, "x" : 5, "v" : 6);
		hash(result_map, source_vector, [](int x) { return string(x); });
		ensure(result_map.compare(("1" : 0, "2" : 0, "3" : 0)));

		// vector->map value+key
		source_vector = (1, 2, 3);
		result_map = ("z" : 4, "x" : 5, "v" : 6);
		hash(result_map, source_vector, [](int x, int i) { return string(i + x); });
		ensure(result_map.compare(("1" : 0, "3" : 0, "5" : 0)));

		// map->vector value
		source_map = ("a" : 1, "b" : 2, "c" : 3);
		result_vector = ("a", "b", "c");
		hash(result_vector, source_map, [](int x) { return string(x); });
		ensure(result_vector.compare(("1", "2", "3")));

		// map->vector value+key
		source_map = ("a" : 1, "b" : 2, "c" : 3);
		result_vector = ("a", "b", "c");
		hash(result_vector, source_map, [](int x, string k) { return k + string(x); });
		ensure(result_vector.compare(("a1", "b2", "c3")));

		// map->map value
		source_map = ("a" : 1, "b" : 2, "c" : 3);
		result_map = ("z" : 4, "x" : 5, "v" : 6);
		hash(result_map, source_map, [](int x) { return string(x); });
		ensure(result_map.compare(("1" : 0, "2" : 0, "3" : 0)));

		// map->map value+key
		source_map = ("a" : 1, "b" : 2, "c" : 3);
		result_map = ("z" : 4, "x" : 5, "v" : 6);
		hash(result_map, source_map, [](int x, string k) { return k + string(x); });
		ensure(result_map.compare(("a1" : 0, "b2" : 0, "c3" : 0)));
	}

	void test_multi_hash()
	{
		string result_vector[0] = ();
		int result_map[] = (
			:);

		// empty first
		result_vector = ("a", "b", "c");
		result_map = ("z" : 4, "x" : 5, "v" : 6);
		hash(result_vector, (), ("q", "w", "e", "r"), [](int x, string y) { return "" });
		hash(result_map, (), ("q", "w", "e", "r"), [](int x, string y) { return "" });
		ensure(result_vector.size() == 0);
		ensure(result_map.size() == 0);

		// empty second
		result_vector = ("a", "b", "c");
		result_map = ("z" : 4, "x" : 5, "v" : 6);
		hash(result_vector, (1, 2, 3), (), [](int x, string y) { return "" });
		hash(result_map, (1, 2, 3), (), [](int x, string y) { return "" });
		ensure(result_vector.size() == 0);
		ensure(result_map.size() == 0);

		// vector+vector -> vector
		result_vector = ("a", "b", "c");
		hash(result_vector, (1, 2, 3), ("q", "w", "e", "r"), [](int x, string y) { return string(x) + y; });
		ensure(result_vector.compare(("1q", "2w", "3e")));

		// map+vector -> vector
		result_vector = ("a", "b", "c");
		hash(result_vector, (1.0f : 1, 2.0f : 2, 3.0f : 3), ("q", "w"), [](int x, string y) { return string(x) + y; });
		ensure(result_vector.compare(("1q", "2w")));

		// vector+map -> map
		result_map = ("z" : 4, "x" : 5, "v" : 6);
		hash(result_map, (1, 2, 3), (1.0f
										: "q", 2.0f
										: "w", 3.0f
										: "e", 4.0f
										: "r"),
			[](int x, string y) { return string(x) + y; });
		ensure(result_map.compare(("1q" : 0, "2w" : 0, "3e" : 0)));
	}

	void test_fold()
	{
		int num;
		string str;

		// empty
		// fold((),[](int a,int b) {});
		// foldRight((),[](int a,int b) {});
		// ok, it throws

		// vector, left, associative op
		num = fold((1, 2, 3, 4), [](int a, int b) { return a + b; });
		ensure(num == 10);
		num = fold((12), [](int a, int b) { return a + b; });
		ensure(num == 12);

		// map, left, associative op
		num = fold(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int a, int b) { return a + b; });
		ensure(num == 10);
		num = fold(("z" : 12), [](int a, int b) { return a + b; });
		ensure(num == 12);

		// vector, right, associative op
		num = foldRight((1, 2, 3, 4), [](int a, int b) { return a + b; });
		ensure(num == 10);
		num = foldRight((12), [](int a, int b) { return a + b; });
		ensure(num == 12)

			// map, right, associative op
			num = foldRight(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int a, int b) { return a + b; });
		ensure(num == 10);
		num = foldRight(("z" : 12), [](int a, int b) { return a + b; });
		ensure(num == 12);

		// vector, left, non-associative op
		str = fold(("k", "n", "o", "b"), [](string a, string b) { return a + b; });
		ensure(str == "knob");
		str = fold(("Y"), [](string a, string b) { return a + b; });
		ensure(str == "Y");

		// map, left, non-associative op
		str = fold((1
					   : "l", 2
					   : "e", 3
					   : "g"),
			[](string a, string b) { return a + b; });
		ensure(str == "leg");
		str = fold((3.14f
					   : "PI"),
			[](string a, string b) { return a + b; });
		ensure(str == "PI");

		// vector, right, non-associative op
		str = foldRight(("k", "n", "o", "b"), [](string a, string b) { return a + b; });
		ensure(str == "bonk");
		str = foldRight(("Y"), [](string a, string b) { return a + b; });
		ensure(str == "Y")

			// map, right, non-associative op
			str = foldRight((1
								: "l", 2
								: "e", 3
								: "g"),
				[](string a, string b) { return a + b; });
		ensure(str == "gel");
		str = foldRight((3.14f
							: "PI"),
			[](string a, string b) { return a + b; });
		ensure(str == "PI");
	}

	void test_reduce()
	{
		int num;
		string str;

		// empty
		num = reduce((), 4, [](int a, int b) {});
		ensure(num == 4);
		num = reduce((
						 :),
			"zero", [](string a, string b) {});
		ensure(num == "zero");
		num = reduceRight((), 4, [](int a, int b) {});
		ensure(num == 4);
		num = reduceRight((
							  :),
			"zero", [](string a, string b) {});
		ensure(num == "zero");

		// vector, left, associative op
		num = reduce((1, 2, 3, 4), 10, [](int a, int b) { return a + b; });
		ensure(num == 20);

		num = reduce((1, 2, 3, 4), 10, [](int a, int b, int key) { return a + b + key; });
		ensure(num == 26);

		// map, left, associative op
		num = reduce(("a" : 1, "b" : 2, "c" : 3, "d" : 4), 10, [](int a, int b) { return a + b; });
		ensure(num == 20);

		num = reduce(("aa" : 1, "bbb" : 2, "c" : 3, "ddddd" : 4), 10, [](int a, int b, string key) { return a + b + strlen(key); });
		ensure(num == 31);

		// vector, right, associative op
		num = reduceRight((1, 2, 3, 4), 10, [](int a, int b) { return a + b; });
		ensure(num == 20);

		num = reduceRight((1, 2, 3, 4), 10, [](int a, int b, int key) { return a + b + key; });
		ensure(num == 26);

		// map, right, associative op
		num = reduceRight(("a" : 1, "b" : 2, "c" : 3, "d" : 4), 10, [](int a, int b) { return a + b; });
		ensure(num == 20);

		num = reduceRight(("aa" : 1, "bbb" : 2, "c" : 3, "ddddd" : 4), 10, [](int a, int b, string key) { return a + b + strlen(key); });
		ensure(num == 31);

		// vector, left, non-associative op
		str = reduce(("k", "n", "o", "b"), "this is ", [](string a, string b) { return a + b; });
		ensure(str == "this is knob");

		str = reduce(("k", "n", "o", "b"), "this is ", [](string a, string b, int key) { return a + b + string(key + 2); });
		ensure(str == "this is k2n3o4b5");

		// map, left, non-associative op
		str = reduce((5.0f
						 : "k", 10.0f
						 : "n", 15.0f
						 : "o", 20.0f
						 : "b"),
			"this is ", [](string a, string b) { return a + b; });
		ensure(str == "this is knob");

		str = reduce((5.0f
						 : "k", 10.0f
						 : "n", 15.0f
						 : "o", 20.0f
						 : "b"),
			"this is ", [](string a, string b, float key) { return a + b + string(int(key / 5.0f)); });
		ensure(str == "this is k1n2o3b4");

		// // vector, right, non-associative op
		str = reduceRight(("k", "n", "o", "b"), "gong ", [](string a, string b) { return a + b; });
		ensure(str == "gong bonk");

		str = reduceRight(("k", "n", "o", "b"), "gong ", [](string a, string b, int key) { return a + b + string(key); });
		ensure(str == "gong b3o2n1k0");

		// // map, right, non-associative op
		str = reduceRight((5.0f
							  : "k", 10.0f
							  : "n", 15.0f
							  : "o", 20.0f
							  : "b"),
			"gong ", [](string a, string b) { return a + b; });
		ensure(str == "gong bonk");

		str = reduceRight((5.0f
							  : "k", 10.0f
							  : "n", 15.0f
							  : "o", 20.0f
							  : "b"),
			"gong ", [](string a, string b, float key) { return a + b + string(int(key / 5.0f)); });
		ensure(str == "gong b4o3n2k1");
	}

	void test_filter()
	{
		int vector[0] = ();
		int map[] = (
			:);
		int removed;

		// vector, in place
		vector = ();
		removed = filter(vector, [](int v) { return true; });
		ensure(removed == 0);
		ensure(vector.size() == 0);

		vector = (1, 2, 3, 4, 5, 6);
		removed = filter(vector, [](int v) { return v % 2 == 0; });
		ensure(removed == 3);
		ensure(vector.compare((2, 4, 6)));

		vector = (1, 2, 3, 4, 5, 6);
		removed = filter(vector, [](int v, int k) { return k % 2 == 0; });
		ensure(removed == 3);
		ensure(vector.compare((1, 3, 5)));

		// map, in place
		map = (
			:);
		removed = filter(map, [](int v) { return true; });
		ensure(removed == 0);
		ensure(map.size() == 0);

		map = ("a" : 1, "b" : 0, "c" : 0, "d" : 0, "e" : 1);
		removed = filter(map);
		ensure(removed == 3);
		ensure(map.compare(("a" : 1, "e" : 1)));

		map = ("a" : 1, "b" : 0, "c" : 0, "d" : 0, "e" : 1);
		removed = filter(map, [](int v, string k) { return v || k == "c"; });
		ensure(removed == 2);
		ensure(map.compare(("a" : 1, "c" : 0, "e" : 1)));

		// vector->vector
		vector = (1, 2, 3);
		filter(vector, (), [](int v) { return true; });
		ensure(vector.size() == 0);

		vector = (1, 2, 3);
		filter(vector, (1, 2, 3, 4, 5, 6), [](int v) { return v % 2 == 0; });
		ensure(vector.compare((2, 4, 6)));

		vector = (1, 2, 3);
		filter(vector, (1, 2, 3, 4, 5, 6), [](int v, int k) { return k % 2 == 0; });
		ensure(vector.compare((1, 3, 5)));

		// vector->map
		map = ("sheesh" : 12);
		filter(map, (), [](int v) { return true; });
		ensure(map.size() == 0);

		map = ("sheesh" : 12);
		filter(map, (1, 2, 3, 4, 5, 6), [](int v) { return v % 2 == 0; });
		ensure(map.compare((1 : 2, 3 : 4, 5 : 6)));

		map = ("sheesh" : 12);
		filter(map, (1, 2, 3, 4, 5, 6), [](int v, int k) { return k % 2 == 0; });
		ensure(map.compare((0 : 1, 2 : 3, 4 : 5)));

		// map->vector
		vector = (1, 2, 3);
		filter(vector, (
						   :),
			[](int v) { return true; });
		ensure(vector.size() == 0);

		vector = (1, 2, 3);
		filter(vector, ("a" : 1, "b" : 2, "c" : 3, "d" : 4, "e" : 5), [](int v) { return v % 2 == 0; });
		ensure(vector.compare((2, 4)));

		vector = (1, 2, 3);
		filter(vector, ("a" : 1, "b" : 2, "c" : 3, "d" : 4, "e" : 5), [](int v, string k) { return v % 2 == 0 || k == "c"; });
		ensure(vector.compare((2, 3, 4)));

		// map->map
		map = ("sheesh" : 12);
		filter(map, (
						:),
			[](int v) { return true; });
		ensure(map.size() == 0);

		map = ("sheesh" : 12);
		filter(map, ("a" : 1, "b" : 0, "c" : 0, "d" : 0, "e" : 1), [](int v) { return v; });
		ensure(map.compare(("a" : 1, "e" : 1)));

		map = ("sheesh" : 12);
		filter(map, ("a" : 1, "b" : 0, "c" : 0, "d" : 0, "e" : 1), [](int v, string k) { return v || k == "c"; });
		ensure(map.compare(("a" : 1, "c" : 0, "e" : 1)));
	}

	void test_reject()
	{
		int vector[0] = ();
		int map[] = (
			:);
		int removed;

		// vector, in place
		vector = ();
		removed = reject(vector, [](int v) { return false; });
		ensure(removed == 0);
		ensure(vector.size() == 0);

		vector = (1, 2, 3, 4, 5, 6);
		removed = reject(vector, [](int v) { return v % 2 == 1; });
		ensure(removed == 3);
		ensure(vector.compare((2, 4, 6)));

		vector = (1, 2, 3, 4, 5, 6);
		removed = reject(vector, [](int v, int k) { return k % 2 == 1; });
		ensure(removed == 3);
		ensure(vector.compare((1, 3, 5)));

		// map, in place
		map = (
			:);
		removed = reject(map, [](int v) { return false; });
		ensure(removed == 0);
		ensure(map.size() == 0);

		map = ("a" : 1, "b" : 0, "c" : 0, "d" : 0, "e" : 1);
		removed = reject(map);
		ensure(removed == 2);
		ensure(map.compare(("b" : 0, "c" : 0, "d" : 0)));

		map = ("a" : 1, "b" : 0, "c" : 0, "d" : 0, "e" : 1);
		removed = reject(map, [](int v, string k) { return !v && k != "c"; });
		ensure(removed == 2);
		ensure(map.compare(("a" : 1, "c" : 0, "e" : 1)));

		// vector->vector
		vector = (1, 2, 3);
		reject(vector, (), [](int v) { return false; });
		ensure(vector.size() == 0);

		vector = (1, 2, 3);
		reject(vector, (1, 2, 3, 4, 5, 6), [](int v) { return v % 2 == 1; });
		ensure(vector.compare((2, 4, 6)));

		vector = (1, 2, 3);
		reject(vector, (1, 2, 3, 4, 5, 6), [](int v, int k) { return k % 2 == 1; });
		ensure(vector.compare((1, 3, 5)));

		// vector->map
		map = ("sheesh" : 12);
		reject(map, (), [](int v) { return false; });
		ensure(map.size() == 0);

		map = ("sheesh" : 12);
		reject(map, (1, 2, 3, 4, 5, 6), [](int v) { return v % 2 == 1; });
		ensure(map.compare((1 : 2, 3 : 4, 5 : 6)));

		map = ("sheesh" : 12);
		reject(map, (1, 2, 3, 4, 5, 6), [](int v, int k) { return k % 2 == 1; });
		ensure(map.compare((0 : 1, 2 : 3, 4 : 5)));

		// map->vector
		vector = (1, 2, 3);
		reject(vector, (
						   :),
			[](int v) { return false; });
		ensure(vector.size() == 0);

		vector = (1, 2, 3);
		reject(vector, ("a" : 1, "b" : 2, "c" : 3, "d" : 4, "e" : 5), [](int v) { return v % 2 == 1; });
		ensure(vector.compare((2, 4)));

		vector = (1, 2, 3);
		reject(vector, ("a" : 1, "b" : 2, "c" : 3, "d" : 4, "e" : 5), [](int v, string k) { return v % 2 == 1 && k != "c"; });
		ensure(vector.compare((2, 3, 4)));

		// map->map
		map = ("sheesh" : 12);
		reject(map, (
						:),
			[](int v) { return false; });
		ensure(map.size() == 0);

		map = ("sheesh" : 12);
		reject(map, ("a" : 1, "b" : 0, "c" : 0, "d" : 0, "e" : 1), [](int v) { return !v; });
		ensure(map.compare(("a" : 1, "e" : 1)));

		map = ("sheesh" : 12);
		reject(map, ("a" : 1, "b" : 0, "c" : 0, "d" : 0, "e" : 1), [](int v, string k) { return !v && k != "c"; });
		ensure(map.compare(("a" : 1, "c" : 0, "e" : 1)));
	}

	void test_find()
	{
		int num;

		// vector, value
		num = find(());
		ensure(num == -1);

		num = find((0, 0, 0, 0));
		ensure(num == -1);

		num = find((1, 2, 3, 4), [](int v) { return v % 2 == 0; });
		ensure(num == 2);

		num = find((1, 2, 3, 4), [](int v) { return v == 10; });
		ensure(num == -1);

		num = find((1, 2, 3, 4), [](int v) { return v == 10; }, 33);
		ensure(num == 33);

		// vector, value+key
		num = find((1, 2, 3, 4), [](int v, int k) { return k == 3; });
		ensure(num == 4);

		num = find((1, 2, 3, 4), [](int v, int k) { return v == 10 || k == 10; });
		ensure(num == -1);

		num = find((1, 2, 3, 4), [](int v, int k) { return k == 10; }, 33);
		ensure(num == 33);

		// map, value
		num = find((
			:));
		ensure(num == -1);

		num = find(("a" : 0, "b" : 0, "c" : 0, "d" : 0));
		ensure(num == -1);

		num = find(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v) { return v % 2 == 0; });
		ensure(num == 2);

		num = find(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v) { return v == 10; });
		ensure(num == -1);

		num = find(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v) { return v == 10; }, 33);
		ensure(num == 33);

		// map, value+key
		num = find(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v, string k) { return k == "d"; });
		ensure(num == 4);

		num = find(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v, string k) { return v == 10 || k == "z"; });
		ensure(num == -1);

		num = find(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v, string k) { return k == "z"; }, 33);
		ensure(num == 33);
	}

	void test_find_key()
	{
		int num;
		string str;

		// vector, value
		num = findKey(());
		ensure(num == -1);

		num = findKey((0, 0, 0, 0));
		ensure(num == -1);

		num = findKey((1, 2, 3, 4), [](int v) { return v % 2 == 0; });
		ensure(num == 1);

		num = findKey((1, 2, 3, 4), [](int v) { return v == 10; });
		ensure(num == -1);

		num = findKey((1, 2, 3, 4), [](int v) { return v == 10; }, 33);
		ensure(num == 33);

		// vector, value+key
		num = findKey((1, 2, 3, 4), [](int v, int k) { return k == 3; });
		ensure(num == 3);

		num = findKey((1, 2, 3, 4), [](int v, int k) { return v == 10 || k == 10; });
		ensure(num == -1);

		num = findKey((1, 2, 3, 4), [](int v, int k) { return k == 10; }, 33);
		ensure(num == 33);

		// map, value
		num = findKey((
			:));
		ensure(num == -1);

		num = findKey(("a" : 0, "b" : 0, "c" : 0, "d" : 0));
		ensure(num == -1);

		str = findKey(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v) { return v % 2 == 0; });
		ensure(str == "b");

		num = findKey(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v) { return v == 10; });
		ensure(num == -1);

		str = findKey(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v) { return v == 10; }, "fail");
		ensure(str == "fail");

		// map, value+key
		str = findKey(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v, string k) { return k == "d"; });
		ensure(str == "d");

		num = findKey(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v, string k) { return v == 10 || k == "z"; });
		ensure(num == -1);

		str = findKey(("a" : 1, "b" : 2, "c" : 3, "d" : 4), [](int v, string k) { return k == "z"; }, "fail");
		ensure(str == "fail");
	}

	void test_any()
	{
		// vectors
		ensure(!any(()));
		ensure(any((1, 3, 5)));
		ensure(!any((0, 0, 0, 0)));
		ensure(!any((1, 3, 5), [](int v) { return v % 2 == 0; }));
		ensure(any((1, 3, 4, 5), [](int v) { return v % 2 == 0; }));
		ensure(any((1, 3, 5), [](int v, int k) { return v % 2 == 0 || k == 2; }));
		ensure(!any((1, 3, 5), [](int v, int k) { return v % 2 == 0 || k == 10; }));

		// map
		ensure(!any((
			:)));
		ensure(any(("a" : 1, "b" : 3, "c" : 5)));
		ensure(!any(("a" : 0, "b" : 0, "c" : 0, "d" : 0)));
		ensure(!any(("a" : 1, "b" : 3, "c" : 5), [](int v) { return v % 2 == 0; }));
		ensure(any(("a" : 1, "b" : 3, "c" : 4, "d" : 5), [](int v) { return v % 2 == 0; }));
		ensure(any(("a" : 1, "b" : 3, "c" : 5), [](int v, string k) { return v % 2 == 0 || k == "c"; }));
		ensure(!any(("a" : 1, "b" : 3, "c" : 5), [](int v, string k) { return v % 2 == 0 || k == "z"; }));
	}

	void test_every()
	{
		// vectors
		ensure(every(()));
		ensure(every((1, 3, 5)));
		ensure(!every((0, 0, 0, 0)));
		ensure(every((1, 3, 5), [](int v) { return v % 2 == 1; }));
		ensure(!every((1, 3, 4, 5), [](int v) { return v % 2 == 1; }));
		ensure(every((1, 3, 4), [](int v, int k) { return v % 2 == 1 || k == 2; }));
		ensure(!every((1, 3, 5), [](int v, int k) { return v % 2 == 0 || k == 10; }));

		// map
		ensure(every((
			:)));
		ensure(every(("a" : 1, "b" : 3, "c" : 5)));
		ensure(!every(("a" : 0, "b" : 0, "c" : 0, "d" : 0)));
		ensure(every(("a" : 1, "b" : 3, "c" : 5), [](int v) { return v % 2 == 1; }));
		ensure(!every(("a" : 1, "b" : 3, "c" : 4, "d" : 5), [](int v) { return v % 2 == 1; }));
		ensure(every(("a" : 1, "b" : 3, "c" : 4), [](int v, string k) { return v % 2 == 1 || k == "c"; }));
		ensure(!every(("a" : 1, "b" : 3, "c" : 5), [](int v, string k) { return v % 2 == 0 || k == "z"; }));
	}

	void test_reverse()
	{
		int vector[0] = ();

		// reverse(("a":1,"b":2));
		// reverse(vector,("a":1,"b":2));
		// int map[] = (:);
		// reverse(map,(1,2,3));
		// ok, it throws

		vector = ();
		reverse(vector);
		ensure(vector.size() == 0);

		vector = (1, 2, 3);
		reverse(vector);
		ensure(vector.compare((3, 2, 1)));

		vector = (33);
		reverse(vector, ());
		ensure(vector.size() == 0);

		vector = (33);
		reverse(vector, (1, 2, 3));
		ensure(vector.compare((3, 2, 1)));
	}

	void test_contains()
	{
		// vector
		ensure(!contains((), 1));
		ensure(contains((1, 2, 3), 1));
		ensure(!contains((1, 2, 3), 33));

		// map
		ensure(!contains((
							 :),
			1));
		ensure(contains(("a" : 1, "b" : 2, "c" : 3), 1));
		ensure(!contains(("a" : 1, "b" : 2, "c" : 3), 33));
	}

	void test_remove()
	{
		int vector[0] = ();
		int map[] = (
			:);
		int removed;

		// vector, in place
		vector = ();
		removed = remove(vector, 4);
		ensure(removed == 0);
		ensure(vector.size() == 0);

		vector = (2, 3, 1, 1, 4, 1, 6);
		removed = remove(vector, 1);
		ensure(removed == 3);
		ensure(vector.compare((2, 3, 4, 6)));

		// map, in place
		map = (
			:);
		removed = remove(map, 4);
		ensure(removed == 0);
		ensure(map.size() == 0);

		map = ("a" : 2, "b" : 3, "c" : 1, "d" : 1, "e" : 4, "f" : 1, "g" : 6);
		removed = remove(map, 1);
		ensure(removed == 3);
		ensure(map.compare(("a" : 2, "b" : 3, "e" : 4, "g" : 6)));

		// vector->vector
		vector = (1, 2, 3);
		remove(vector, (), 4);
		ensure(vector.size() == 0);

		vector = (1, 2, 3);
		remove(vector, (2, 3, 1, 1, 4, 1, 6), 1);
		ensure(vector.compare((2, 3, 4, 6)));

		// vector->map
		map = ("a" : 1, "b" : 2, "c" : 3);
		remove(map, (), 4);
		ensure(map.size() == 0);

		map = ("a" : 1, "b" : 2, "c" : 3);
		remove(map, (2, 3, 1, 1, 4, 1, 6), 1);
		ensure(map.compare((0 : 2, 1 : 3, 4 : 4, 6 : 6)));

		// map->vector
		vector = (1, 2, 3);
		remove(vector, (
						   :),
			4);
		ensure(vector.size() == 0);

		vector = (1, 2, 3);
		remove(vector, ("a" : 2, "b" : 3, "c" : 1, "d" : 1, "e" : 4, "f" : 1, "g" : 6), 1);
		ensure(vector.compare((2, 3, 4, 6)));

		// map->map
		map = ("a" : 1, "b" : 2, "c" : 3);
		remove(map, (
						:),
			4);
		ensure(map.size() == 0);

		map = ("a" : 1, "b" : 2, "c" : 3);
		remove(map, ("a" : 2, "b" : 3, "c" : 1, "d" : 1, "e" : 4, "f" : 1, "g" : 6), 1);
		ensure(map.compare(("a" : 2, "b" : 3, "e" : 4, "g" : 6)));
	}

#undef ensure

}; /* class Underscore */

} /* namespace Unigine */

#endif /* __UNIGINE_UNDERSCORE_H__ */
