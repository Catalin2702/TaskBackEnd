//
// Created by Catalin Chirosca on 08/12/24.
//

#ifndef COLUMN_HPP
#define COLUMN_HPP

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <map>
#include <vector>
#include <pqxx/pqxx>
#include <sstream>
#include <type_traits>

#include "tools/tools.hpp"

namespace value {

	enum class SqlType {
		Serial,
		Integer,
		String,
		Text,
		Decimal,
		Date,
		Time,
		DateTime,
		Boolean,
		Enum,
		Timestamp,
	};

	inline std::map<SqlType, std::string> SqlTypeDef {
		{SqlType::Serial, "SERIAL"},
		{SqlType::Integer, "INTEGER"},
		{SqlType::String, "STRING"},
		{SqlType::Text, "TEXT"},
		{SqlType::Decimal, "DECIMAL"},
		{SqlType::Date, "DATE"},
		{SqlType::Time, "TIME"},
		{SqlType::DateTime, "DATETIME"},
		{SqlType::Boolean, "BOOLEAN"},
		{SqlType::Enum, "ENUM"},
		{SqlType::Timestamp, "TIMESTAMP"},
	};

	inline std::map<SqlType, bool> SqlTypeStringLike {
		{SqlType::Serial, false},
		{SqlType::Integer, false},
		{SqlType::String, true},
		{SqlType::Text, true},
		{SqlType::Decimal, false},
		{SqlType::Date, true},
		{SqlType::Time, true},
		{SqlType::DateTime, true},
		{SqlType::Boolean, true},
		{SqlType::Enum, true},
		{SqlType::Timestamp, true},
	};

	class SqlValue {
	public:
		explicit SqlValue(const SqlType type);
		virtual ~SqlValue() = default;
		[[nodiscard]] virtual std::string toString() const = 0;
		[[nodiscard]] SqlType getType() const;
		[[nodiscard]] static bool isStringLike();
	private:
		SqlType type;
	};

	class SerialValue final : public SqlValue {
	public:
		explicit SerialValue(unsigned int value);
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] unsigned int* getPtr();
		void setValue(unsigned int value);
		
	private:
		unsigned int value;
	};

	class IntegerValue final : public SqlValue {
	public:
		explicit IntegerValue(int value);
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] int* getPtr();
		void setValue(int value);
		
	private:
		int value;
	};

	class StringValue final : public SqlValue {
	public:
		explicit StringValue(std::string value);
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] std::string* getPtr();
		void setValue(const std::string& value);
		[[nodiscard]] static bool isStringLike();
		
	private:
		std::string value;
	};

	class TextValue final : public SqlValue {
	public:
		explicit TextValue(std::string value);
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] std::string* getPtr();
		void setValue(const std::string& value);
		[[nodiscard]] static bool isStringLike();
		
	private:
		std::string value;
	};

	class DecimalValue final : public SqlValue {
	public:
		explicit DecimalValue(double value);
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] double* getPtr();
		void setValue(double value);
		
	private:
		double value;
	};

	class DateValue final : public SqlValue {
	public:
		explicit DateValue(const std::chrono::system_clock::time_point& value);
		explicit DateValue(const std::string & dateStr, const std::string& format = "%Y-%m-%d");
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] std::string toString(const std::string& format) const;
		[[nodiscard]] std::chrono::system_clock::time_point* getPtr();
		void setValue(const std::chrono::system_clock::time_point& value);
		void setValue(const std::string& dateStr, const std::string& format = "%Y-%m-%d");
		[[nodiscard]] static bool isStringLike();
		
	private:
		std::chrono::system_clock::time_point value;
	};

	class TimeValue final : public SqlValue {
	public:
		explicit TimeValue(const std::chrono::seconds& value);
		explicit TimeValue(const std::string& timeStr, const std::string& format = "%H:%M:%S");
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] std::string toString(const std::string& format) const;
		[[nodiscard]] std::chrono::seconds* getPtr();
		void setValue(const std::chrono::seconds& value);
		void setValue(const std::string& timeStr, const std::string& format = "%H:%M:%S");
		[[nodiscard]] static bool isStringLike();
		
	private:
		std::chrono::seconds value;
	};

	class DateTimeValue final : public SqlValue {
	public:
		explicit DateTimeValue(const std::chrono::system_clock::time_point& value);
		explicit DateTimeValue(const std::string& dateTimeStr, const std::string& format = "%Y-%m-%d %H:%M:%S");
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] std::string toString(const std::string& format) const;
		[[nodiscard]] std::chrono::system_clock::time_point* getPtr();
		void setValue(const std::chrono::system_clock::time_point& value);
		void setValue(const std::string& dateTimeStr, const std::string& format = "%Y-%m-%d %H:%M:%S");
		[[nodiscard]] static bool isStringLike();
		
	private:
		std::chrono::system_clock::time_point value;
	};

	class TimestampValue final : public SqlValue {
	public:
		explicit TimestampValue(const std::chrono::system_clock::time_point& value);
		explicit TimestampValue(const std::string& timestampStr, const std::string& format = "%Y-%m-%d %H:%M:%S");
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] std::string toString(const std::string& format) const;
		[[nodiscard]] std::chrono::system_clock::time_point* getPtr();
		void setValue(const std::chrono::system_clock::time_point& value);
		void setValue(const std::string& timestampStr, const std::string& format = "%Y-%m-%d %H:%M:%S");
		[[nodiscard]] static bool isStringLike();
		
	private:
		std::chrono::system_clock::time_point value;
	};

	class BooleanValue final : public SqlValue {
	public:
		explicit BooleanValue(const bool value);
		explicit BooleanValue(const int value);
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] bool* getPtr();
		void setValue(const bool value);
		void setValue(const int value);
		
	private:
		bool value;
	};

	template<typename T>
	class EnumValue final : public SqlValue {
	public:
		explicit EnumValue(const std::map<T, std::string>& mapping, const T* value = nullptr);
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] const std::map<T, std::string>& getMapping() const;
		[[nodiscard]] T* getPtr();
		void setValue(const T value);
		void setMapping(const std::map<T, std::string>& mapping, const T* value = nullptr);
		[[nodiscard]] static bool isStringLike();
		
	private:
		T value;
		std::map<T, std::string> mapping;
	};
}

namespace condition {

	enum class SqlOperator {
		AND,
		OR,
		NOT,
		EQUAL,
		NOT_EQUAL,
		LESS,
		LESS_EQUAL,
		GREATER,
		GREATER_EQUAL,
		IN,
		NOT_IN,
		LIKE,
		NOT_LIKE,
		ILIKE,
		NOT_ILIKE,
		IS_NULL,
		IS_NOT_NULL,
		IS,
		IS_NOT,
		IS_TRUE,
		IS_FALSE,
	};

	inline std::map<SqlOperator, std::string> SqlOperatorDef {
		{SqlOperator::AND, "AND"},
		{SqlOperator::OR, "OR"},
		{SqlOperator::NOT, "NOT"},
		{SqlOperator::EQUAL, "="},
		{SqlOperator::NOT_EQUAL, "!="},
		{SqlOperator::LESS, "<"},
		{SqlOperator::LESS_EQUAL, "<="},
		{SqlOperator::GREATER, ">"},
		{SqlOperator::GREATER_EQUAL, ">="},
		{SqlOperator::IN, "IN"},
		{SqlOperator::NOT_IN, "NOT IN"},
		{SqlOperator::LIKE, "LIKE"},
		{SqlOperator::ILIKE, "ILIKE"},
		{SqlOperator::NOT_ILIKE, "NOT ILIKE"},
		{SqlOperator::NOT_LIKE, "NOT LIKE"},
		{SqlOperator::IS_NULL, "IS NULL"},
		{SqlOperator::IS_NOT_NULL, "IS NOT NULL"},
		{SqlOperator::IS, "IS"},
		{SqlOperator::IS_NOT, "IS NOT"},
		{SqlOperator::IS_TRUE, "IS TRUE"},
		{SqlOperator::IS_FALSE, "IS FALSE"},
	};

	class ConditionBase {
	public:
		virtual ~ConditionBase() = default;
		[[nodiscard]] virtual std::string toSql() const = 0;
		[[nodiscard]] virtual std::string toString() const = 0;
		[[nodiscard]] virtual std::unique_ptr<ConditionBase> clone() const = 0;
	};

	template<typename T>
	class Condition final: public ConditionBase {
	public:
		Condition(const std::string& column, const SqlOperator op, const T value);
		Condition(const std::string& column, const SqlOperator op, const std::vector<T>& values);
		explicit Condition(std::unique_ptr<ConditionBase> conditionBase);
		Condition(const std::string& column, const SqlOperator op, const T value, std::unique_ptr<ConditionBase> composition);
		Condition(const std::string& column, const SqlOperator op, const std::vector<T>& values, std::unique_ptr<ConditionBase> composition);
		Condition(Condition&& other) noexcept = default;
		Condition& operator=(Condition&& other) noexcept = default;
		Condition(const Condition&) = delete;
		Condition& operator=(const Condition&) = delete;
		[[nodiscard]] std::string toSql() const override;
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] std::unique_ptr<ConditionBase> clone() const override;
		explicit operator std::string() const;
		Condition operator && (const ConditionBase& other) const;
		Condition operator || (const ConditionBase& other) const;
		Condition operator ! () const;
	private:
		std::string column;
		SqlOperator op;
		std::vector<T> values;
		std::unique_ptr<ConditionBase> composition;
		bool isComposite = false;
	};

	class CompositeCondition final: public ConditionBase {
	public:
		CompositeCondition(std::unique_ptr<ConditionBase> left, std::unique_ptr<ConditionBase> right, const SqlOperator op);
		[[nodiscard]] std::string toSql() const override;
		[[nodiscard]] std::string toString() const override;
		[[nodiscard]] std::unique_ptr<ConditionBase> clone() const override;
		explicit operator std::string() const;
	private:
		std::unique_ptr<ConditionBase> left;
		std::unique_ptr<ConditionBase> right;
		SqlOperator op;
	};

}

namespace column {

	class ColumnBase {
	public:
		ColumnBase(std::string name, const value::SqlType type, std::string tableName = "", const bool primaryKey = false, const bool nullable = true);
		virtual ~ColumnBase() = default;
		[[nodiscard]] std::string getName() const;
		[[nodiscard]] std::string getTableName() const;
		[[nodiscard]] std::string getFullName() const;
		[[nodiscard]] value::SqlType getType() const;
		[[nodiscard]] std::string getTypeDefinition() const;
		template<typename T>
		[[nodiscard]] T* getPtrValue() const;
		[[nodiscard]] virtual std::string getValueAsString() const = 0;
		[[nodiscard]] bool isPrimaryKey() const;
		[[nodiscard]] bool isNullable() const;
		[[nodiscard]] std::string toString() const;
		void setTableName(const std::string& tableName);
		virtual void setNull(const bool init = false) = 0;
		virtual void initValue(const pqxx::field& field) = 0;
		virtual void setValueFromPtr(const void* ptr) = 0;
		[[nodiscard]] virtual std::unique_ptr<ColumnBase> clone() const = 0;
		[[nodiscard]] bool isPtrNull() const;
		[[nodiscard]] bool isPtrNotNull() const;
		[[nodiscard]] static bool isStringLike();
		bool isDirty() const;
		void markDirty();
		void clearDirty();
		[[nodiscard]] condition::Condition<std::string> isNull() const;
		[[nodiscard]] condition::Condition<std::string> isNotNull() const;
		explicit operator std::string() const;
	protected:
		[[nodiscard]] virtual void* getPtr() const = 0;
		std::string name;
		std::string tableName;
		value::SqlType type;
		bool primaryKey;
		bool nullable;
		bool dirty = false;
	private:
		void setDirty(const bool dirty);
	};

	class SerialColumn final: public ColumnBase {
	public:
		typedef int value_type;
		explicit SerialColumn(const std::string& name, const bool primaryKey = false, const bool nullable = true);
		explicit SerialColumn(const std::string& name, const unsigned int value, const bool primaryKey = false, const bool nullable = false);
		SerialColumn(const SerialColumn& other);
		~SerialColumn() override;
		[[nodiscard]] unsigned int* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const unsigned int value);
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		SerialColumn& operator = (const unsigned int value);
		condition::Condition<unsigned int> operator == (const unsigned int value) const;
		condition::Condition<unsigned int> operator != (const unsigned int value) const;
		condition::Condition<unsigned int> operator < (const unsigned int value) const;
		condition::Condition<unsigned int> operator <= (const unsigned int value) const;
		condition::Condition<unsigned int> operator > (const unsigned int value) const;
		condition::Condition<unsigned int> operator >= (const unsigned int value) const;
		[[nodiscard]] condition::Condition<unsigned int> in(const std::vector<unsigned int>& values) const;
		[[nodiscard]] condition::Condition<unsigned int> notIn(const std::vector<unsigned int>& values) const;

	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::SerialValue> value;
	};

	class IntegerColumn final: public ColumnBase {
	public:
		typedef int value_type;
		explicit IntegerColumn(const std::string& name, const bool primaryKey = false, const bool nullable = true);
		explicit IntegerColumn(const std::string& name, const int value, const bool primaryKey = false, const bool nullable = true);
		IntegerColumn(const IntegerColumn& other);
		~IntegerColumn() override;
		[[nodiscard]] int* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const int value);
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		IntegerColumn& operator = (const int value);
		condition::Condition<int> operator == (const int value) const;
		condition::Condition<int> operator != (const int value) const;
		condition::Condition<int> operator < (const int value) const;
		condition::Condition<int> operator <= (const int value) const;
		condition::Condition<int> operator > (const int value) const;
		condition::Condition<int> operator >= (const int value) const;
		[[nodiscard]] condition::Condition<int> in(const std::vector<int>& values) const;
		[[nodiscard]] condition::Condition<int> notIn(const std::vector<int>& values) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::IntegerValue> value;
	};

	class StringColumn final: public ColumnBase {
	public:
		typedef std::string value_type;
		explicit StringColumn(const std::string& name, const size_t length = 255, const bool primaryKey = false, const bool nullable = true);
		explicit StringColumn(const std::string& name, const std::string& value, const size_t length = 255, const bool primaryKey = false, const bool nullable = true);
		StringColumn(const StringColumn& other);
		~StringColumn() override;
		[[nodiscard]] std::string* getPtrValue() const;
		[[nodiscard]] size_t getLength() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const std::string& value);
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		[[nodiscard]] static bool isStringLike();
		StringColumn& operator = (const std::string& value);
		StringColumn& operator = (const char* value);
		condition::Condition<std::string> operator == (const std::string& value) const;
		condition::Condition<std::string> operator != (const std::string& value) const;
		condition::Condition<std::string> operator < (const std::string& value) const;
		condition::Condition<std::string> operator <= (const std::string& value) const;
		condition::Condition<std::string> operator > (const std::string& value) const;
		condition::Condition<std::string> operator >= (const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> like(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> iLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notILike(const std::string& value) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::StringValue> value;
		size_t length;
	};

	class TextColumn final: public ColumnBase {
	public:
		typedef std::string value_type;
		explicit TextColumn(const std::string& name, const bool primaryKey = false, const bool nullable = true);
		explicit TextColumn(const std::string& name, const std::string& value, const bool primaryKey = false, const bool nullable = true);
		TextColumn(const TextColumn& other);
		~TextColumn() override;
		[[nodiscard]] std::string* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const std::string& value);
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		[[nodiscard]] static bool isStringLike();
		TextColumn& operator = (const std::string& value);
		TextColumn& operator = (const char* value);
		condition::Condition<std::string> operator == (const std::string& value) const;
		condition::Condition<std::string> operator != (const std::string& value) const;
		condition::Condition<std::string> operator < (const std::string& value) const;
		condition::Condition<std::string> operator <= (const std::string& value) const;
		condition::Condition<std::string> operator > (const std::string& value) const;
		condition::Condition<std::string> operator >= (const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> like(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> iLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notILike(const std::string& value) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::TextValue> value;
	};

	class DecimalColumn final: public ColumnBase {
	public:
		typedef double value_type;
		explicit DecimalColumn(const std::string& name, const bool primaryKey = false, const bool nullable = true);
		explicit DecimalColumn(const std::string& name, const double value, const bool primaryKey = false, const bool nullable = true);
		DecimalColumn(const DecimalColumn& other);
		~DecimalColumn() override;
		[[nodiscard]] double* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const double value);
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		DecimalColumn& operator = (const double value);
		condition::Condition<double> operator == (const double value) const;
		condition::Condition<double> operator != (const double value) const;
		condition::Condition<double> operator < (const double value) const;
		condition::Condition<double> operator <= (const double value) const;
		condition::Condition<double> operator > (const double value) const;
		condition::Condition<double> operator >= (const double value) const;
		[[nodiscard]] condition::Condition<double> in(const std::vector<double>& values) const;
		[[nodiscard]] condition::Condition<double> notIn(const std::vector<double>& values) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::DecimalValue> value;
	};

	class DateColumn final: public ColumnBase {
	public:
		typedef std::chrono::system_clock::time_point value_type;
		explicit DateColumn(const std::string& name, const bool primaryKey = false, const bool nullable = true);
		explicit DateColumn(const std::string& name, const std::chrono::system_clock::time_point& value, const bool primaryKey = false, const bool nullable = true);
		explicit DateColumn(const std::string& name, const std::string& dateStr, const std::string& format = "%Y-%m-%d", const bool primaryKey = false, const bool nullable = true);
		DateColumn(const DateColumn& other);
		~DateColumn() override;
		[[nodiscard]] std::chrono::system_clock::time_point* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const std::chrono::system_clock::time_point& value);
		void setValue(const std::string& dateStr, const std::string& format = "%Y-%m-%d");
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		[[nodiscard]] static bool isStringLike();
		DateColumn& operator = (const std::chrono::system_clock::time_point& value);
		DateColumn& operator = (const std::string& dateStr);
		DateColumn& operator = (const char* dateStr);
		condition::Condition<std::string> operator == (const std::string& value) const;
		condition::Condition<std::string> operator == (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator != (const std::string& value) const;
		condition::Condition<std::string> operator != (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator < (const std::string& value) const;
		condition::Condition<std::string> operator < (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator <= (const std::string& value) const;
		condition::Condition<std::string> operator <= (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator > (const std::string& value) const;
		condition::Condition<std::string> operator > (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator >= (const std::string& value) const;
		condition::Condition<std::string> operator >= (const std::chrono::system_clock::time_point& value) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector <std::chrono::system_clock::time_point>& value) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector <std::chrono::system_clock::time_point>& value) const;
		[[nodiscard]] condition::Condition<std::string> like(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> iLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notILike(const std::string& value) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::DateValue> value;
	};

	class TimeColumn final: public ColumnBase {
	public:
		typedef std::chrono::seconds value_type;
		explicit TimeColumn(const std::string& name, const bool primaryKey = false, const bool nullable = true);
		explicit TimeColumn(const std::string& name, const std::chrono::seconds& value, const bool primaryKey = false, const bool nullable = true);
		explicit TimeColumn(const std::string& name, const std::string& timeStr, const std::string& format = "%H-%M-%S", const bool primaryKey = false, const bool nullable = true);
		TimeColumn(const TimeColumn& other);
		~TimeColumn() override;
		[[nodiscard]] std::chrono::seconds* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const std::chrono::seconds& value);
		void setValue(const std::string& timeStr, const std::string& format = "%H-%M-%S");
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		[[nodiscard]] static bool isStringLike();
		TimeColumn& operator = (const std::chrono::seconds& value);
		TimeColumn& operator = (const std::string& timeStr);
		TimeColumn& operator = (const char* timeStr);
		condition::Condition<std::string> operator == (const std::string& value) const;
		condition::Condition<std::string> operator == (const std::chrono::seconds& value) const;
		condition::Condition<std::string> operator != (const std::string& value) const;
		condition::Condition<std::string> operator != (const std::chrono::seconds& value) const;
		condition::Condition<std::string> operator < (const std::string& value) const;
		condition::Condition<std::string> operator < (const std::chrono::seconds& value) const;
		condition::Condition<std::string> operator <= (const std::string& value) const;
		condition::Condition<std::string> operator <= (const std::chrono::seconds& value) const;
		condition::Condition<std::string> operator > (const std::string& value) const;
		condition::Condition<std::string> operator > (const std::chrono::seconds& value) const;
		condition::Condition<std::string> operator >= (const std::string& value) const;
		condition::Condition<std::string> operator >= (const std::chrono::seconds& value) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector <std::chrono::seconds>& value) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector <std::chrono::seconds>& value) const;
		[[nodiscard]] condition::Condition<std::string> like(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> iLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notILike(const std::string& value) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::TimeValue> value;
	};

	class DateTimeColumn final: public ColumnBase {
	public:
		typedef std::chrono::system_clock::time_point value_type;
		explicit DateTimeColumn(const std::string& name, const bool primaryKey = false, const bool nullable = true);
		explicit DateTimeColumn(const std::string& name, const std::chrono::system_clock::time_point& value, const bool primaryKey = false, const bool nullable = true);
		explicit DateTimeColumn(const std::string& name, const std::string& dateTimeStr, const std::string& format = "%Y-%m-%d %H:%M:%S", const bool primaryKey = false, const bool nullable = true);
		DateTimeColumn(const DateTimeColumn& other);
		~DateTimeColumn() override;
		[[nodiscard]] std::chrono::system_clock::time_point* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const std::chrono::system_clock::time_point& value);
		void setValue(const std::string& dateTimeStr, const std::string& format = "%Y-%m-%d %H:%M:%S");
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		[[nodiscard]] static bool isStringLike();
		DateTimeColumn& operator = (const std::chrono::system_clock::time_point& value);
		DateTimeColumn& operator = (const std::string& dateTimeStr);
		DateTimeColumn& operator = (const char* dateTimeStr);
		condition::Condition<std::string> operator == (const std::string& value) const;
		condition::Condition<std::string> operator == (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator != (const std::string& value) const;
		condition::Condition<std::string> operator != (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator < (const std::string& value) const;
		condition::Condition<std::string> operator < (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator <= (const std::string& value) const;
		condition::Condition<std::string> operator <= (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator > (const std::string& value) const;
		condition::Condition<std::string> operator > (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator >= (const std::string& value) const;
		condition::Condition<std::string> operator >= (const std::chrono::system_clock::time_point& value) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector <std::chrono::system_clock::time_point>& value) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector <std::chrono::system_clock::time_point>& value) const;
		[[nodiscard]] condition::Condition<std::string> like(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> iLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notILike(const std::string& value) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::DateTimeValue> value;
	};

	class TimestampColumn final: public ColumnBase {
	public:
		typedef std::chrono::system_clock::time_point value_type;
		explicit TimestampColumn(const std::string& name, const bool primaryKey = false, const bool nullable = true);
		explicit TimestampColumn(const std::string& name, const std::chrono::system_clock::time_point& value, const bool primaryKey = false, const bool nullable = true);
		explicit TimestampColumn(const std::string& name, const std::string& dateTimeStr, const std::string& format = "%Y-%m-%d %H:%M:%S", const bool primaryKey = false, const bool nullable = true);
		TimestampColumn(const TimestampColumn& other);
		~TimestampColumn() override;
		[[nodiscard]] std::chrono::system_clock::time_point* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const std::chrono::system_clock::time_point& value);
		void setValue(const std::string& timestampStr, const std::string& format = "%Y-%m-%d %H:%M:%S");
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		[[nodiscard]] static bool isStringLike();
		TimestampColumn& operator = (const std::chrono::system_clock::time_point& value);
		TimestampColumn& operator = (const std::string& timestampStr);
		TimestampColumn& operator = (const char* timestampStr);
		condition::Condition<std::string> operator == (const std::string& value) const;
		condition::Condition<std::string> operator == (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator != (const std::string& value) const;
		condition::Condition<std::string> operator != (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator < (const std::string& value) const;
		condition::Condition<std::string> operator < (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator <= (const std::string& value) const;
		condition::Condition<std::string> operator <= (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator > (const std::string& value) const;
		condition::Condition<std::string> operator > (const std::chrono::system_clock::time_point& value) const;
		condition::Condition<std::string> operator >= (const std::string& value) const;
		condition::Condition<std::string> operator >= (const std::chrono::system_clock::time_point& value) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector <std::chrono::system_clock::time_point>& value) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector <std::chrono::system_clock::time_point>& value) const;
		[[nodiscard]] condition::Condition<std::string> like(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> iLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notILike(const std::string& value) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::TimestampValue> value;
	};

	class BooleanColumn final: public ColumnBase {
	public:
		typedef bool value_type;
		explicit BooleanColumn(const std::string& name, const bool primaryKey = false, const bool nullable = true);
		explicit BooleanColumn(const std::string& name, const bool value, const bool primaryKey = false, const bool nullable = true);
		explicit BooleanColumn(const std::string& name, const int value, const bool primaryKey = false, const bool nullable = true);
		BooleanColumn(const BooleanColumn& other);
		~BooleanColumn() override;
		[[nodiscard]] bool* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const bool value);
		void setValue(const int value);
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		BooleanColumn& operator = (const bool value);
		BooleanColumn& operator = (const int value);
		condition::Condition<bool> operator == (const bool value) const;
		condition::Condition<bool> operator == (const int value) const;
		condition::Condition<bool> operator != (const bool value) const;
		condition::Condition<bool> operator != (const int value) const;
		[[nodiscard]] condition::Condition<bool> in(const std::vector<bool>& values) const;
		[[nodiscard]] condition::Condition<bool> in(const std::vector<int>& values) const;
		[[nodiscard]] condition::Condition<bool> notIn(const std::vector<bool>& values) const;
		[[nodiscard]] condition::Condition<bool> notIn(const std::vector<int>& values) const;
		[[nodiscard]] condition::Condition<bool> is(const bool value) const;
		[[nodiscard]] condition::Condition<bool> isNot(const bool value) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::BooleanValue> value = nullptr;
	};
	
	template <typename T>
	class EnumColumn final: public ColumnBase {
	public:
		typedef T value_type;
		explicit EnumColumn(const std::string& name, const std::map<T, std::string> mapping, const bool primaryKey = false, const bool nullable = true);
		explicit EnumColumn(const std::string& name, const T value, const std::map<T, std::string> mapping,   const bool primaryKey = false, const bool nullable = true);
		EnumColumn(const EnumColumn& other);
		~EnumColumn() override;
		[[nodiscard]] const std::map<T, std::string>& getMapping() const;
		[[nodiscard]] T* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const override;
		void setValue(const T value);
		void setMapping(const std::map<T, std::string>& mapping, const T value);
		void setNull(const bool init = false) override;
		void initValue(const pqxx::field& field) override;
		void setValueFromPtr(const void* ptr) override;
		[[nodiscard]] std::unique_ptr<ColumnBase> clone() const override;
		[[nodiscard]] static bool isStringLike();
		EnumColumn& operator = (const T value);
		condition::Condition<std::string> operator == (const T value) const;
		condition::Condition<std::string> operator == (const std::string& value) const;
		condition::Condition<std::string> operator != (const T value) const;
		condition::Condition<std::string> operator != (const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector<T>& values) const;
		[[nodiscard]] condition::Condition<std::string> in(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector<T>& values) const;
		[[nodiscard]] condition::Condition<std::string> notIn(const std::vector<std::string>& values) const;
		[[nodiscard]] condition::Condition<std::string> like(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> iLike(const std::string& value) const;
		[[nodiscard]] condition::Condition<std::string> notILike(const std::string& value) const;
	protected:
		[[nodiscard]] void* getPtr() const override;
	private:
		std::unique_ptr<value::EnumValue<T>> value;
	};

	template<typename T>
	class ColumnRef {
		std::shared_ptr<T> column;
		typedef typename T::value_type value_type;

	public:
		explicit ColumnRef(std::shared_ptr<T> col);

		operator std::shared_ptr<T>();
		operator std::shared_ptr<T>&();
		T* operator->() const;
		T& operator*() const;

		// Operatori di assegnamento 
		ColumnRef& operator = (const value_type& value);

		// Per StringColumn abbiamo bisogno di una specializzazione specifica per const char*
		template<typename U = T>
		ColumnRef& operator = (const char* value);

		// Operatori di confronto (esempio per StringColumn)
		condition::Condition<value_type> operator == (const value_type& value) const;
		condition::Condition<value_type> operator != (const value_type& value) const;
		condition::Condition<value_type> operator < (const value_type& value) const;
		condition::Condition<value_type> operator <= (const value_type& value) const;
		condition::Condition<value_type> operator > (const value_type& value) const ;
		condition::Condition<value_type> operator >= (const value_type& value) const;

		std::shared_ptr<T>& get();
		value_type* getPtrValue() const;
		[[nodiscard]] std::string getValueAsString() const;
		void setNull(const bool init = false);

		// Metodi specifici per stringhe
		template<typename U = T>
		std::enable_if_t<std::is_base_of_v<ColumnBase, U> && U::isStringLike(),
			condition::Condition<std::string>>
		in(const std::vector<std::string>& values) const;

		template<typename U = T>
		std::enable_if_t<std::is_base_of_v<ColumnBase, U> && U::isStringLike(),
			condition::Condition<std::string>>
		notIn(const std::vector<std::string>& values) const;
		template<typename U = T>
		std::enable_if_t<std::is_base_of_v<ColumnBase, U> && U::isStringLike(),
			condition::Condition<std::string>>
		like(const std::string& value) const;
		template<typename U = T>
		std::enable_if_t<std::is_base_of_v<ColumnBase, U> && U::isStringLike(),
			condition::Condition<std::string>>
		notLike(const std::string& value) const;
		template<typename U = T>
		std::enable_if_t<std::is_base_of_v<ColumnBase, U> && U::isStringLike(),
			condition::Condition<std::string>>
		iLike(const std::string& value) const;
		template<typename U = T>
		std::enable_if_t<std::is_base_of_v<ColumnBase, U> && U::isStringLike(),
			condition::Condition<std::string>>
		notILike(const std::string& value) const;
	};

}

#include "column.tpp"

#endif //COLUMN_HPP
