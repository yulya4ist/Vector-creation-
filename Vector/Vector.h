#pragma once
#include<string>
#include<iostream>

namespace mcv// My Class Vector
{
	constexpr size_t BUFFER_SIZE = 16; 

	class vector_exception : public std::exception
	{
	public:
		vector_exception(std::string msg) :msg(msg) {}
		const char* what() const noexcept { return msg.c_str(); }
	private:
		std::string msg;
	};

	struct linear_generator_t {
	private:
		double step;
		double begin;
	public:
		linear_generator_t(double value, double st) {//инициализация начальным значением и шагом
			begin = value;
			step = st;
		}
		double operator()() { return begin += step; }
	};

	struct power_generator_t {
	private:
		double begin;
	public:
		power_generator_t(double value) {
			begin = value;
		}
		double operator()() {
			return begin *= begin;
		}
	};

	struct random_generator_t {
	public:
		random_generator_t() {}
		double operator()() {
			return rand();
		}
	};



	template <typename T>
	class Vector
	{
	public:

		class Iterator;
		Iterator begin() { return data; }
		Iterator end() { return (data + size ); }
		//Конструкторы:
		Vector();
		Vector(size_t size);
		Vector(size_t size, const T& default_element);
		Vector(const Vector<T>& other);
		Vector(Vector<T>&& other);
		//Методы:
		T& At(size_t index);
		const T& At(size_t index) const;
		size_t Size() const { return this->size; }
		size_t Capacity() const { return this->capacity; }
		//Операторы: 
		Vector<T>& operator=(const Vector<T>& other);
		Vector<T>& operator=(Vector<T>&& other);
		T& operator[](size_t index) noexcept;
		const T& operator[](size_t index) const noexcept;
	    Vector<T> operator+() const;
		Vector<T> operator-() const;
		Vector<T> operator*(T value) const;
		const Vector<T> operator+(const Vector<T> & other);
		const Vector<T> operator-(const Vector<T>& other);
		//const Vector<T> operator+( Vector<T>&& other);
		//const Vector<T> operator-(Vector<T>&& other);

		
		void print() const; 
		void fill_random(Iterator it_begin, Iterator it_end); 

		void fill(Vector<T>& v, double value, double step) {
			for (int i = 0; i < v.size(); ++i) {
				v[i] = value + step * i;
			}
		}

		void fill(Iterator it_begin, Iterator it_end, double value, double step) {
			int i = 0; double val = 0;
			for (it_begin; it_begin != it_end; ++it_begin) {
				val = value + step * i;
				++i;
				*it_begin = val;
			}
		}

		void fill_step_linear(Iterator it_begin, Iterator it_end, double value, double step) {
			int i = 0; double val = 0;
			for (it_begin; it_begin != it_end; ++it_begin) {
				val = value + step * i;
				++i;
				*it_begin = val;
			}
		}

		void fill_step_power(Iterator it_begin, Iterator it_end, double value) {
			double val = value;
			for (it_begin; it_begin != it_end; ++it_begin) {
				val *= val;
				*it_begin = val;
			}
		}

		//-------------------generator----------------------
		void fill(Vector<T>& v, linear_generator_t& gen) {
			for (int i = 0; i < v.size(); ++i) {
				v[i] = gen();
			}
		}
		void fill(Iterator it_begin, Iterator it_end, linear_generator_t& gen) {
			for (it_begin; it_begin != it_end; ++it_begin) {
				*it_begin = gen();
			}
		}

		//power
		void fill(Vector<T>& v, power_generator_t& gen) {
			for (int i = 0; i < v.size(); ++i) {
				v[i] = gen();
			}
		}
		void fill(Iterator it_begin, Iterator it_end, power_generator_t& gen) {
			for (it_begin; it_begin != it_end; ++it_begin) {
				*it_begin = gen();
			}
		}

		//произвольно с генератором
		void fill(Vector<T>& v, random_generator_t& gen); 
		void fill(Iterator it_begin, Iterator it_end, random_generator_t& gen); 
		void init() { size = 0; data = nullptr; }
		void clear() { delete[] data; size = 0; data = nullptr; }
		//Деструктор:
		~Vector();

		class Iterator
		{
			T* cur;
		public:
			Iterator(T* first) : cur(first) {}

			T& operator+ (int n) { return *(cur + n); }
			T& operator- (int n) { return *(cur - n); }
			T& operator++ (int) { return *(cur ++ ); };
			T& operator-- (int) { return *(cur--); };
			T& operator++ () { return *(++cur); };
			T& operator-- () { return *(--cur); };

			bool operator!= (const Iterator& it) { return cur != it.cur; }
			bool operator== (const Iterator& it) { return cur == it.cur; }

			T& operator* () { return *cur; }
		};
	private:
		T* data;
		size_t size;
		size_t capacity; 
	};

	template<typename T>
	Vector<T>::Vector() :Vector(0) {}

	template<typename T>
	Vector<T>::Vector(size_t size) :size(size), capacity(BUFFER_SIZE + size)
	{
		try
		{
			data = new T[this->capacity];
		}
		catch (std::bad_alloc & exc)
		{
			throw vector_exception("vector too long");
		}
	}

	template<typename T>
	Vector<T>::Vector(size_t size, const T& default_element) :Vector(size)
	{
		for (size_t i = 0; i < this->size; i++)
		{
			data[i] = default_element;
		}
	}

	template<typename T>
	Vector<T>::Vector(const Vector<T>& other): Vector(other.size)
	{
		for (size_t i = 0; i < other.size; i++)
		{
			this->data[i] = other.data[i]; 
		}
	}

	template<typename T>
	Vector<T>::Vector(Vector<T>&& other)
	{
		this->data = other.data;
		this->capacity = other.capacity;
		this->size = other.size;
		other.data = nullptr;
	}

	template<typename T>
	T& Vector<T>::At(size_t index)
	{
		if (index > this->size)
			throw vector_exception("invalid vector subscript");
		return this->data[index];
	}

	template<typename T>
	const T& Vector<T>::At(size_t index) const
	{
		if (index > this->size)
			throw vector_exception("invalid vector subscript");
		return this->data[index];
	}

	template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector<T>& other)
	{
		if (this == &other)
			return *this;
		if (this->capacity < other.size)
		{
			delete[] this->data;
			this->capacity = other.capacity;
			try
			{
				data = new T[this->capacity];
			}
			catch (std::bad_alloc & exc)
			{
				throw vector_exception("vector too long");
			}
		}
		for (size_t i = 0; i < other.size; i++)
		{
			this->data[i] = other.data[i];
		}
		this->size = other.size;
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector<T>&& other)
	{
		if (this == &other)
			return *this;
		delete[] this->data;
		this->data = other.data;
		this->capacity = other.capacity;
		this->size = other.size;
		other.data = nullptr;
		return *this;
	}

	template<typename T>
	T& Vector<T>::operator[](size_t index) noexcept
	{
		return this->data[index];
	}

	template<typename T>
	const T& Vector<T>::operator[](size_t index) const noexcept
	{
		return this->data[index];
	}


	template<typename T>
	Vector<T> Vector<T>::operator+() const
	{
		return *this;
	}

	template<typename T>
	Vector<T> Vector<T>::operator-() const
	{
		Vector<T> negative_this = Vector<T>(*this);
		for (size_t i = 0; i < negative_this.size; i++)
		{
			negative_this.data[i] = -negative_this.data[i];
		}
		return negative_this;
	}

	template<typename T>
	Vector<T> Vector<T>::operator*(T value) const 
	{
		Vector<T> multiply_this = Vector<T>(*this);
		for (size_t i = 0; i < multiply_this.size; i++)
		{
			multiply_this.data[i] *= value;
		}
		return multiply_this;
	}

	template<typename T>
	const Vector<T> Vector<T>::operator+(const Vector<T>& other)
	{
		if (this->size != other.size)
			throw vector_exception("vectors of different sizes");
		
		for (size_t i = 0; i < this->size; i++)
		{
			this->data[i] += other[i];
		}
		return *this;
	}

	template<typename T>
	const Vector<T> Vector<T>::operator-(const Vector<T>& other)
	{
		if (this->size != other.size)
			throw vector_exception("vectors of different sizes");
		//Vector<T> buf = *this;

		for (size_t i = 0; i < this->size; i++)
		{
			this->data[i] -= other[i];
		}
		return *this;
	}

	template<typename T>
	void Vector<T>::print() const
	{
			for (int i = 0; i < size; ++i) {
				std::cout << data[i] << ' ';
			}	
	}

	template<typename T>
	void Vector<T>::fill_random(Iterator it_begin, Iterator it_end)
	{
			for (it_begin; it_begin != it_end; ++it_begin) {
				*it_begin = rand();
			}
	}

	/*template<typename T>
	Vector<T> Vector<T>::operator+(Vector<T>&& other)
	{
		if (this->size != other.size)
			throw vector_exception("vectors of different sizes");
		
		for (size_t i = 0; i < this->size; i++)
		{
			this->data[i] += other[i];
		}
		return *this;
	}

	template<typename T>
	Vector<T> Vector<T>::operator-(Vector<T>&& other)
	{
		if (this->size != other.size)
			throw vector_exception("vectors of different sizes");

		Vector<T> buf = *this;

		for (size_t i = 0; i < this->size; i++)
		{
			this->data[i] -= other[i];
		}
		return *this;
	}*/

	template<typename T>
	inline void Vector<T>::fill(Vector<T>& v, random_generator_t& gen)
	{
			for (int i = 0; i < v.size(); ++i) {
				v[i] = gen();
			}
	}

	template<typename T>
	inline void Vector<T>::fill(Iterator it_begin, Iterator it_end, random_generator_t& gen)
	{
			for (it_begin; it_begin != it_end; ++it_begin) {
				*it_begin = gen();
			}
	}

	template<typename T>
	Vector<T>::~Vector()
	{
		delete[] data;
	}

	template<typename T>
	Vector<T> operator*(T value, const Vector<T>& vec)
	{
		Vector<T> multiply_vec = Vector<T>(vec);
		for (size_t i = 0; i < multiply_vec.Size(); i++)
		{
			multiply_vec[i] *= value;
		}
		return multiply_vec;
	}
}

