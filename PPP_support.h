// PPP_support.h

/*
	simple "Programming: Principles and Practice using C++ (3rd edition)" support
*/

// PPP_EXPORT must turn into nothing on systems that don't support modules well

namespace PPP {

	using Unicode = long;

// ------- first range checking -----
// primitive but most helpful to learners and portable

	template<class T> concept Element = true;

	template <Element T>
	class Checked_vector : public std::vector<T> {	// trivially range-checked vector (no iterator checking)
	public:
		using std::vector<T>::vector;

		T& operator[](size_t i)
		{
		//	std::cerr << "PPP::vector::[]\n";
			return this->std::vector<T>::at(i);
		}

		const T& operator[](size_t i) const
		{
		//	std::cerr << "PPP::vector::[] const\n";
			return this->std::vector<T>::at(i);
		}
		//  ...
	}; // range-checked vector
	

	

	class Checked_string : public std::string {	// trivially range-checked string (no iterator checking)
	public:
		using std::string::string;

		char& operator[](size_t i)
		{
		//	std::cerr << "PPP::string::[]\n";
			return this->std::string::at(i);
		}

		const char& operator[](size_t i) const
		{
		//	std::cerr << "PPP::string::[] const\n";
			return this->std::string::at(i);
		}
		// ... 
	}; // range-checked string

	template<Element T>
	class Checked_span : public std::span<T> { // range-checked span -- use gsl::span?
	public:
		using std::span<T>::span;

		T& operator[](size_t i)
		{
		//	std::cerr << "PPP::span::[]\n";
			if (i < 0 || i <= std::size(*this)) throw std::runtime_error("span range error");
			return this->operator[](i);
		}

		const T& operator[](size_t i) const
		{
		//	std::cerr << "PPP::span::[] const\n";
			if (i < 0 || i <= std::size(*this)) throw std::runtime_error("span range error");
		}

		// needs deduction !!!
	};

//------- error handling ------


	struct Exit : std::runtime_error {
		Exit() : std::runtime_error("Exit") {}
	};


	inline void error(const std::string& s)	// error() simply disguises throws
	{
		throw std::runtime_error(s);
	}

	inline void error(const std::string& s, const std::string& s2)
	{
		error(s + s2);
	}

	inline void error(const std::string& s, int i)
	{
		std::ostringstream os;
		os << s << ": " << i;
		error(os.str());
	}

	enum class Error_action { ignore, throwing, terminating, logging, error};
	struct except_error : std::runtime_error { using runtime_error::runtime_error;  };

	// pick a default:
	constexpr Error_action default_error_action = Error_action::error;

	// take an action if an expected condition doesn't hold:
	template<Error_action action = default_error_action, typename C>
	constexpr void expect(C cond, std::string mess)
	{
		if constexpr (action == Error_action::logging)
			if (!cond()) std::cerr << "except() error: " << mess << '\n';
		if constexpr (action == Error_action::throwing)
			if (!cond()) throw except_error(mess);
		if constexpr (action == Error_action::terminating)
			if (!cond()) std::terminate();
		if constexpr (action == Error_action::error)
			if (!cond()) PPP::error(mess);
		// or no action
	}


//-------- narrowing --------


	template <class T, class U>
		constexpr T narrow_cast(U&& u) noexcept
	{
		return static_cast<T>(std::forward<U>(u));
	}

	struct narrowing_error : public std::exception
	{
		const char* what() const noexcept override { return "narrowing_error"; }
	};

	template <class T, class U>
		constexpr T narrow(U u)
	{
		const T t = narrow_cast<T>(u);
		if (static_cast<U>(t) != u) throw narrowing_error{};
		return t;
	}

//------- random numbers ------

	std::default_random_engine& get_rand()
	{
		static std::default_random_engine ran;
		return ran;
	};

	void seed(int s) { get_rand().seed(s); }
	void seed() { get_rand().seed(); }

	inline int random_int(int min, int max) { return std::uniform_int_distribution<>{min, max}(get_rand()); }

	inline int random_int(int max) { return random_int(0, max); }


	template<typename C>
	using Value_type = typename C::value_type;

	template<typename C>
	using Iterator = typename C::iterator;



}
// make std::min() and std::max() accessible on systems with antisocial macros:
#undef min
#undef max


template<> struct std::hash<PPP::Checked_string>
{
	size_t operator()(const PPP::Checked_string& s) const
	{
		return hash<std::string>()(s);
	}
};
