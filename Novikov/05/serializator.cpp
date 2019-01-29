enum class Error {
	NoError,
	CorruptedArchive
};

class Serializer {
	std::ostream& out_;
	static constexpr char Separator = ' ';
	template <class T, class... ArgsTypes>
	Error process(T&& value, ArgsTypes&&... args) {
		if(Error::CorruptedArchive != process(std::forward<T>(value)))
			return process(std::forward<ArgsTypes>(args)...);
		else return Error::CorruptedArchive;
	}
	Error process() {
		return Error::NoError;
	}
	Error process(bool value) {
		out_ << ((value)?("true"):("false")) << Separator;
		return Error::NoError;
	}

	Error process(uint64_t value) {
		out_ << value << Separator;
		return Error::NoError;
	}
public:
	explicit Serializer(std::ostream& out) : out_(out) {}
	template <class T>
	Error save(T&& object) {
		return object.serialize(*this);
	}

	template <class... ArgsTypes>
	Error operator()(ArgsTypes&&... args) {
		return process(std::forward<ArgsTypes>(args)...);
	}
};

class Deserializer {
	std::istream& in_;
	static constexpr char Separator = ' ';
	Error process() {
		return Error::NoError;
	}
 	template <class T, class... ArgsTypes>
	Error process(T&& value, ArgsTypes&&... args) {
		if(Error::CorruptedArchive != process(std::forward<T>(value)))
			return process(std::forward<ArgsTypes>(args)...);
		else return Error::CorruptedArchive;
	}
	Error process(uint64_t& value) {
		std::string s;
		in_>> s;
		if(s != "") {
			for(auto c : s)
				if(!isdigit(c)) return Error::CorruptedArchive;
			value = std::stoull(s);
			return Error::NoError;
		}
		else return Error::CorruptedArchive;
	}
	Error process(bool& value) {
		std::string str;
		in_ >> str;
		if(str == "false") value = false; 
		else if(str == "true") value = true;
		else return Error::CorruptedArchive;
		return Error::NoError;
	}
public:
	explicit Deserializer(std::istream& in) : in_(in) {}
	template <class T>
	Error load(T& object) {
		return object.serialize(*this);
	}
	template <class... ArgsTypes>
	Error operator()(ArgsTypes&&... args) {
		return process(std::forward<ArgsTypes>(args)...);
	}
};
