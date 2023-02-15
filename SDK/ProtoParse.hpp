//https://developers.google.com/protocol-buffers/docs/encoding
#pragma once
#include <cstdint>
#include <string>
#include <vector>

#define MAKE_TAG(FIELD_NUMBER, TYPE) static_cast<uint32_t>(((FIELD_NUMBER) << kTagTypeBits) | (TYPE))

#define make_struct(_name_, _size_) \
	struct _name_ : ProtoWriter { \
		constexpr static size_t MAX_FIELD = _size_; \
		_name_()						: ProtoWriter(MAX_FIELD) {} \
		_name_(void* data, size_t size) : ProtoWriter(data, size, MAX_FIELD) {} \
		_name_(std::string data)        : ProtoWriter(data, MAX_FIELD) {}

#define make_field(_name_, _id_, _type_) \
	constexpr static Tag _name_ = { _id_, _type_ }; \
	void clear_##_name_() { this->clear(_name_); } \
	bool has_##_name_() { return this->has(_name_); } \
	Field get_##_name_() { return this->get(_name_); } \
	std::vector<Field> getAll_##_name_() { return this->getAll(_name_); } \
	\
	void add_##_name_(std::string v) { this->add(_name_, v); } \
	template<typename T> void add_##_name_(T v) { this->add(_name_, v); } \
	void replace_##_name_(std::string v) { this->replace(_name_, v); } \
	void replace_##_name_(std::string v, uint32_t index) { this->replace(_name_, v, index); } \
	template<typename T> void replace_##_name_(T v) { this->replace(_name_, v); } \
	template<typename T> void replace_##_name_(T v, uint32_t index) { this->replace(_name_, v, index); } \
	template<class T> T get_##_name_() { return std::move( T(this->get(_name_).String()) ); }

/*
 * make_struct and make_field make inherited struct from ProtoWriter and add methods for work with fields
 *
 * Example generated struct with one field:
 *
 * make_struct(CMsgClientHello, 8)
 *    make_field(client_session_need, 3, TYPE_UINT32)
 * };
 *
 * struct CMsgClientHello : ProtoWriter {
 *    constexpr static size_t MAX_FIELD = 8;
 *    CMsgClientHello() : ProtoWriter(MAX_FIELD) {}
 *    CMsgClientHello(void* data, size_t size) : ProtoWriter(data, size, MAX_FIELD) {}
 *    CMsgClientHello(std::string data) : ProtoWriter(data, MAX_FIELD) {}
 *
 *    constexpr static Tag client_session_need = { 3, TYPE_UINT32 };
 *
 *    void clear_client_session_need() {
 *		this->clear(client_session_need);
 *	  }
 *
 *    bool has_client_session_need() {
 *       return this->has(client_session_need);
 *    }
 *
 *    Field get_client_session_need() {
 *       return this->get(client_session_need);
 *    }
 *
 *    std::vector<Field> getAll_client_session_need() {
 *       return this->getAll(client_session_need);
 *    }
 *
 *    void add_client_session_need(std::string v) {
 *       this->add(client_session_need, v);
 *    }
 *
 *    template<typename T>
 *    void add_client_session_need(T v) {
 *       this->add(client_session_need, v);
 *    }
 *
 *    void replace_client_session_need(std::string v) {
 *       this->replace(client_session_need, v);
 *    }
 *
 *    void replace_client_session_need(std::string v, uint32_t index) {
 *       this->replace(client_session_need, v, index);
 *    }
 *
 *    template<typename T>
 *    void replace_client_session_need(T v) {
 *       this->replace(client_session_need, v);
 *    }
 *
 *    template<typename T>
 *    void replace_client_session_need(T v, uint32_t index) {
 *       this->replace(client_session_need, v, index);
 *    }
 *
 *    template<class T>
 *    T get_client_session_need() {
 *       return std::move( T(this->get(client_session_need).String()) );
 *    }
 * };
 */


struct Tag
{
	unsigned field;
	unsigned type;
};

enum class FieldType {
	TYPE_DOUBLE = 1,
	TYPE_FLOAT = 2,
	TYPE_INT64 = 3,
	TYPE_UINT64 = 4,
	TYPE_INT32 = 5,
	TYPE_FIXED64 = 6,
	TYPE_FIXED32 = 7,
	TYPE_BOOL = 8,
	TYPE_STRING = 9,
	TYPE_GROUP = 10,
	TYPE_MESSAGE = 11,
	TYPE_BYTES = 12,
	TYPE_UINT32 = 13,
	TYPE_ENUM = 14,
	TYPE_SFIXED32 = 15,
	TYPE_SFIXED64 = 16,
	TYPE_SINT32 = 17,
	TYPE_SINT64 = 18,
	MAX_FieldYPE = 18,
};

struct Field
{
	friend class ProtoWriter;
public:
	inline Field& operator=(const Field& f);

	Field() : tag({ 0,0 }), value(""), full("") { }
	Field(unsigned field, unsigned type, std::string value, std::string full);

	Field(Tag tag, std::string value);
	Field(unsigned field, unsigned type, std::string value);

	template<typename T>
	Field(Tag tag, T value);

	template<typename T>
	Field(unsigned field, unsigned type, T value);

public:
	static Field ReadField(void* data, size_t& bytesRead);

public:
	inline float Float();
	inline double Double();
	inline int32_t Int32();
	inline int64_t Int64();
	inline uint32_t UInt32();
	inline uint64_t UInt64();
	inline uint32_t Fixed32();
	inline uint64_t Fixed64();
	inline int32_t SFixed32();
	inline int64_t SFixed64();
	inline bool Bool();
	inline std::string String();

private:
	Tag tag;
	std::string value;
	std::string full;

	static std::string getBytesVarint32(uint32_t value);
	static std::string getBytesVarint64(uint64_t value);
	static uint32_t readVarUint32(void* data, size_t& bytesRead);
	static uint64_t readVarUint64(void* data, size_t& bytesRead);

	enum WireType {
		WIRETYPE_VARINT = 0,
		WIRETYPE_FIXED64 = 1,
		WIRETYPE_LENGTH_DELIMITED = 2,
		WIRETYPE_START_GROUP = 3,
		WIRETYPE_END_GROUP = 4,
		WIRETYPE_FIXED32 = 5,
	};

	constexpr static WireType kWireTypeForFieldType[18 + 1] = {
		static_cast<WireType>(-1),  // invalid
		WIRETYPE_FIXED64,           // TYPE_DOUBLE
		WIRETYPE_FIXED32,           // TYPE_FLOAT
		WIRETYPE_VARINT,            // TYPE_INT64
		WIRETYPE_VARINT,            // TYPE_UINT64
		WIRETYPE_VARINT,            // TYPE_INT32
		WIRETYPE_FIXED64,           // TYPE_FIXED64
		WIRETYPE_FIXED32,           // TYPE_FIXED32
		WIRETYPE_VARINT,            // TYPE_BOOL
		WIRETYPE_LENGTH_DELIMITED,  // TYPE_STRING
		WIRETYPE_START_GROUP,       // TYPE_GROUP
		WIRETYPE_LENGTH_DELIMITED,  // TYPE_MESSAGE
		WIRETYPE_LENGTH_DELIMITED,  // TYPE_BYTES
		WIRETYPE_VARINT,            // TYPE_UINT32
		WIRETYPE_VARINT,            // TYPE_ENUM
		WIRETYPE_FIXED32,           // TYPE_SFIXED32
		WIRETYPE_FIXED64,           // TYPE_SFIXED64
		WIRETYPE_VARINT,            // TYPE_SINT32
		WIRETYPE_VARINT,            // TYPE_SINT64
	};

	constexpr static int kTagTypeBits = 3;
	constexpr static uint32_t kTagTypeMask = (1 << kTagTypeBits) - 1;
	constexpr static int kMaxVarintBytes = 10;
	constexpr static int kMaxVarint32Bytes = 5;
};

class ProtoWriter
{
public:
	inline ProtoWriter();
	inline ProtoWriter(size_t maxFields);
	inline ProtoWriter(void* data, size_t size, size_t maxFields);
	inline ProtoWriter(std::string dataStr, size_t maxFields);

public:
	/* Old method */
	inline void add(Field field);
	inline void replace(Field field);
	inline void replace(Field field, uint32_t index);
	inline void clear(unsigned fieldId);
	inline bool has(unsigned fieldId);
	inline Field get(unsigned fieldId);
	inline std::vector<Field> getAll(unsigned fieldId);

public:
	/* New method */
	inline void add(Tag tag, std::string value);
	inline void replace(Tag tag, std::string value);
	inline void replace(Tag tag, std::string value, uint32_t index);
	inline void clear(Tag tag);
	inline bool has(Tag tag);
	inline Field get(Tag tag);
	inline std::vector<Field> getAll(Tag tag);

	template<typename T>
	inline void add(Tag tag, T value);

	template<typename T>
	inline void replace(Tag tag, T value);

	template<typename T>
	inline void replace(Tag tag, T value, uint32_t index);


	std::string serialize();
	void print();

private:
	std::vector<std::vector<Field>> fields;
};
