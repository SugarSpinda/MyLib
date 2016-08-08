#ifndef _JSON_HPP_
#define _JSON_HPP_

#include <string>
#include <vector>
#include <list>
#include <map>
#include "common/Exception.hpp"


class JsonArray;
class JsonArrayAst;
class JsonObject;
class JsonObjectAst;


class JsonValue
{
public:
	typedef enum Type
	{
		NULL_VALUE,
		STRING,
		NUMBER,
		OBJECT,
		ARRAY,
		BOOLEAN
	} Type;

	typedef struct Object
	{
		JsonObject *obj;
		JsonObjectAst *ast;
	} Object;

	typedef struct Array
	{
		JsonArray *arr;
		JsonArrayAst *ast;
	} Array;

	JsonValue() : value(nullptr), refCount(new int(1)), type(NULL_VALUE) {}
	JsonValue(const JsonValue &v) { copy(v); }
	~JsonValue() { release(); }

	static std::string typeToString(Type type);

	void assertType(Type type) const;

	inline Type getType() const { return this->type; }

	static JsonValue create(const char *str, size_t length);
	static JsonValue create(const std::string &str) { return create(str.data(), str.length()); }
	static JsonValue create(bool value);
	static JsonValue create(double value);

	void createString(const char *str, size_t length);
	void createString(const std::string &str) { createString(str.data(), str.length()); }

	inline void setString(const std::string &str)
	{
		assertType(STRING);
		*(std::string *)this->value = str;
	}

	inline std::string & getString() const
	{
		assertType(STRING);
		return *(std::string *)this->value;
	}

	void createBoolean(bool value);

	inline void setBoolean(bool value)
	{
		assertType(BOOLEAN);
		this->value = value ? (void *)1 : nullptr;
	}

	inline bool getBoolean() const
	{
		assertType(BOOLEAN);
		return this->value != nullptr;
	}

	void createNumber(double value);

	inline void setNumber(double value)
	{
		assertType(NUMBER);
		*(double *)this->value = value;
	}

	inline double getNumber() const
	{
		assertType(NUMBER);
		return *(double *)this->value;
	}

	void createArray();

	inline const JsonArray * getArray() const
	{
		assertType(ARRAY);
		return ((Array *)this->value)->arr;
	}

	inline JsonArray * getArray()
	{
		assertType(ARRAY);
		return ((Array *)this->value)->arr;
	}

	inline JsonArrayAst * getArrayAst()
	{
		assertType(ARRAY);
		return ((Array *)this->value)->ast;
	}

	void createObject();

	inline JsonObject * getObject() const
	{
		assertType(OBJECT);
		return ((Object *)this->value)->obj;
	}

	inline JsonObjectAst * getObjectAst()
	{
		assertType(OBJECT);
		return ((Object *)this->value)->ast;
	}

	std::string toString() const;

	void assign(const JsonValue &v);

	JsonValue & operator = (const JsonValue &v)
	{ this->assign(v); return *this; }

private:
	void release();
	void copy(const JsonValue &v);

	void *value;
	int *refCount;
	Type type;
};


class JsonArray
{
	std::vector<JsonValue> array;
public:
	void put(const JsonValue &v) { this->array.push_back(v); }
	inline JsonValue & get(int index) { return this->array[(unsigned)index]; }
	inline const JsonValue & get(int index) const { return this->array[(unsigned)index]; }
	inline size_t getSize() const { return this->array.size(); }
	inline JsonValue & operator [] (int index) { return this->get(index); }
	inline const JsonValue & operator [] (int index) const { return this->get(index); }
};


class JsonPair
{
public:
	JsonPair(const std::string &name) : name(name) {}

	inline void setName(const std::string &name) { this->name = name; }
	inline std::string getName() const { return this->name; }

	inline void setValue(const JsonValue &value) { this->value = value; }
	inline JsonValue getValue() const { return this->value; }
	inline JsonValue * getReferenceValue() { return &this->value; }

	inline bool equals(const std::string &name) const { return this->name == name; }

private:
	std::string name;
	JsonValue value;
};


class JsonObject
{
public:
	typedef std::list<JsonPair> Pairs;
	typedef std::map<std::string, JsonValue *> PairMap;

	inline void addPair(const JsonPair &p) { pairs.push_back(p); }
	void removePair(const std::string &name);

	PairMap buildMap();

	inline Pairs::iterator begin() { return pairs.begin(); }
	inline Pairs::iterator end() { return pairs.end(); }
	inline Pairs::const_iterator begin() const { return pairs.begin(); }
	inline Pairs::const_iterator end() const { return pairs.end(); }
	inline JsonPair & front() { return pairs.front(); }
	inline const JsonPair & front() const { return pairs.front(); }
	inline JsonPair & back() { return pairs.back(); }
	inline const JsonPair & back() const { return pairs.back(); }
	inline size_t getPairsSize() const { return pairs.size(); }

private:
	Pairs pairs;
};


class Json
{
	JsonObject object;

	static void newLineToString(std::string &buffer, int tabDepth);
	static void appendSpace(std::string &buffer);
	static void valueToString(std::string &buffer, const JsonValue &value, int tabDepth);
	static void arrayToString(std::string &buffer, const JsonArray &array, int tabDepth);
	static void objectToString(std::string &buffer, const JsonObject &object, int tabDepth);

public:
	typedef JsonObject::Pairs Pairs;
	typedef JsonObject::PairMap PairMap;

	Json() {}
	Json(const char *fileName) { (void)open(fileName); }
	Json(const std::string &fileName) { (void)open(fileName.data()); }
	Json(const void *data, size_t length) { (void)openFromMemory(data, length); }
	Json(const Json &json) : object(json.object) {}
	Json(const JsonObject &object) : object(object) {}

	bool open(const char *fileName);
	bool openFromMemory(const void *data, size_t length);
	void save(const char *fileName);

	std::string toString() const;

	inline void addPair(const JsonPair &p) { this->object.addPair(p); }
	inline void removePair(const std::string &name) { this->object.removePair(name); }

	inline Json & operator = (const Json &json) { this->object = json.object; return *this; }

	inline PairMap buildMap() { return this->object.buildMap(); }

	inline Pairs::iterator begin() { return this->object.begin(); }
	inline Pairs::iterator end() { return this->object.end(); }
	inline Pairs::const_iterator begin() const { return this->object.begin(); }
	inline Pairs::const_iterator end() const { return this->object.end(); }
	inline JsonPair & front() { return this->object.front(); }
	inline const JsonPair & front() const { return this->object.front(); }
	inline JsonPair & back() { return this->object.back(); }
	inline const JsonPair & back() const { return this->object.back(); }
};

#endif
