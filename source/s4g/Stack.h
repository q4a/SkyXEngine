#ifndef s4g_stack_h
#define s4g_stack_h

#define stack_pop(stack,count)\
{\
	if (count > stack.count_obj)\
		stack.count_obj = 0; \
	else\
		stack.count_obj -= count; \
}

template <typename T, int BlockSize = 16>
class s4g_stack
{
public:
	s4g_stack(){ count_obj = 0; Arr.resize(BlockSize); }

	inline void init_size(long csize)
	{
		count_obj = csize;
	}

	inline void push(T val)
	{
		if (Arr.AllocSize > count_obj)
			Arr.Data[count_obj] = val;
		else
		{
			Arr.resize(Arr.AllocSize + BlockSize);
			Arr[count_obj] = val;
		}
		++count_obj;
	}

	inline void push_back(T val)
	{
		if (Arr.AllocSize > count_obj)
			Arr.Data[count_obj] = val;
		else
		{
			Arr.resize(Arr.AllocSize + BlockSize);
			Arr[count_obj] = val;
		}
		++count_obj;
	}

	inline void pop(int count)
	{
		if (count > count_obj)
			count_obj = 0;
		else
			count_obj -= count;
	}

	inline T& get(int id)
	{
		if (id - 1 >= 0 && id - 1 < count_obj)
		{
			return Arr.Data[id - 1];
		}
		else if (id < 0)
		{
			if (abs(id) - 1 < count_obj)
			{
				//count_obj = (count_obj + id) + 1;
				return Arr.Data[count_obj + id];
			}
			else
			{
				int qwert = 0;
			}
		}
		else if (id > count_obj && id <= Arr.AllocSize)
		{
			count_obj = id;
			return Arr.Data[id - 1];
		}
		else  if (id > Arr.AllocSize)
		{
			Arr.resize(Arr.AllocSize + BlockSize);
			return Arr[id - 1];
		}
		else
		{
			int qwert = 0;
		}
	}

	inline T & operator[](int id)
	{
		if (id >= 0 && id < count_obj)
		{
			if (!Arr.Data[id])
				int qwert = 0;
			return Arr.Data[id];
		}
		else if (id < 0)
		{
			if (abs(id) < count_obj)
			{
				//count_obj = (count_obj + id) + 1;
				return Arr.Data[count_obj + id];
			}
			else
			{
				int qwert = 0;
			}
		}
		else if (id >= count_obj && id <= Arr.AllocSize)
		{
			count_obj = id+1;
			return Arr.Data[id];
		}
		else  if (id > Arr.AllocSize)
		{
			Arr.resize(Arr.AllocSize + BlockSize);
			return Arr[id];
		}
		else
		{
			int qwert = 0;
		}
	}

	inline long count()
	{
		return count_obj;
	}

	inline long size()
	{
		return count_obj;
	}

	inline void clear()
	{
		Arr.clear();
	}

//protected:
	long count_obj;
	Array<T, BlockSize> Arr;
};

template<typename T, int BlockSize = 16>
class s4g_stack_register
{
public:
	s4g_stack_register(){
		last_free = 0;
	}

	inline long count()
	{
		return last_free;
	}

	//���������� �������� ������� ����������
	inline void setn_first_free(T val)
	{
		if (Arr.AllocSize > last_free)
			Arr.Data[last_free] = val;
		else
			Arr[last_free] = val;
		++last_free;
	}

	//���������� �������� ���������� ������������
	inline int setn_last_unfree(T val)
	{
		if (last_free > 0)
		{
			Arr[last_free - 1] = val;
			return 0;
		}
		return 0;
	}

	//���������� ��������� �����������
	inline void free_last_unfree()
	{
		if (last_free > 0)
			--last_free;
	}

	//�������� ����� ������� ����������
	inline long getn_first_free()
	{
		return last_free;
	}

	//�������� ����� ������� ������������
	inline long getn_last_unfree()
	{
		return last_free - 1;
	}

	//�������� �������� ������� ����������
	inline T getv_first_free()
	{
		if (last_free > 0)
			return Arr[last_free];
	}

	//�������� �������� ���������� ������������
	inline T getv_last_unfree()
	{
		if (last_free > 0)
			return Arr[last_free - 1];
	}

	//�������� ��������
	inline T getn(long id)
	{
		if (id < Arr.size())
			return Arr[id];
	}

	//���������� ��������
	inline int setn(long id, T val)
	{
		if (id >= 0)
		{
			Arr[id] = val;
			return 1;
		}
		else
			return 0;
	}

protected:
	long last_free;
	Array<T, BlockSize> Arr;
};

#endif