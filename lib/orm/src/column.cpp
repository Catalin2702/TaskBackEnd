//
// Created by Catalin Chirosca on 08/12/24.
//

#include <utility>

#include "orm/column.hpp"
#include "tools/tools.hpp"

namespace value {

	SqlValue::SqlValue(const SqlType type): type(type) {}
	SqlType SqlValue::getType() const {
		return type;
	}
	bool SqlValue::isStringLike() {
		return false;
	}

	SerialValue::SerialValue(const unsigned int value):
		SqlValue(SqlType::Serial), value(value) {}
	std::string SerialValue::toString() const {
		return std::to_string(value);
	}
	unsigned int* SerialValue::getPtr() {
		return &value;
	}
	void SerialValue::setValue(const unsigned int value) {
		this->value = value;
	}

	IntegerValue::IntegerValue(const int value):
		SqlValue(SqlType::Integer), value(value) {}
	std::string IntegerValue::toString() const {
		return std::to_string(value);
	}
	int* IntegerValue::getPtr() {
		return &value;
	}
	void IntegerValue::setValue(const int value) {
		this->value = value;
	}

	StringValue::StringValue(std::string value):
		SqlValue(SqlType::String), value(std::move(value)) {}
	std::string StringValue::toString() const {
		return value;
	}
	std::string* StringValue::getPtr() {
		return &value;
	}
	void StringValue::setValue(const std::string &value) {
		this->value = value;
	}
	bool StringValue::isStringLike() {
		return true;
	}

	TextValue::TextValue(std::string value):
		SqlValue(SqlType::Text), value(std::move(value)) {}
	std::string TextValue::toString() const {
		return value;
	}
	std::string* TextValue::getPtr() {
		return &value;
	}
	void TextValue::setValue(const std::string &value) {
		this->value = value;
	}
	bool TextValue::isStringLike() {
		return true;
	}

	DecimalValue::DecimalValue(const double value):
		SqlValue(SqlType::Decimal), value(value) {}
	std::string DecimalValue::toString() const {
		return std::to_string(value);
	}
	double* DecimalValue::getPtr() {
		return &value;
	}
	void DecimalValue::setValue(const double value) {
		this->value = value;
	}

	DateValue::DateValue(const std::chrono::system_clock::time_point &value):
		SqlValue(SqlType::Date), value(value) {}
	DateValue::DateValue(const std::string & dateStr, const std::string& format):
		SqlValue(SqlType::Date), value(tools::stringToDate(dateStr, format)) {}
	std::string DateValue::toString() const {
		return tools::dateToString(value);
	}
	std::string DateValue::toString(const std::string &format) const {
		return tools::dateToString(value, format);
	}
	std::chrono::system_clock::time_point* DateValue::getPtr() {
		return &value;
	}
	void DateValue::setValue(const std::chrono::system_clock::time_point &value) {
		this->value = value;
	}
	void DateValue::setValue(const std::string &dateStr, const std::string &format) {
		this->value = tools::stringToDate(dateStr, format);
	}
	bool DateValue::isStringLike() {
		return true;
	}

	TimeValue::TimeValue(const std::chrono::seconds &value):
		SqlValue(SqlType::Time), value(value) {}
	TimeValue::TimeValue(const std::string &timeStr, const std::string &format):
		SqlValue(SqlType::Time), value(tools::stringToTime(timeStr, format)) {}
	std::string TimeValue::toString() const {
		return tools::timeToString(value);
	}
	std::string TimeValue::toString(const std::string &format) const {
		return tools::timeToString(value, format);
	}
	std::chrono::seconds* TimeValue::getPtr() {
		return &value;
	}
	void TimeValue::setValue(const std::chrono::seconds &value) {
		this->value = value;
	}
	void TimeValue::setValue(const std::string &timeStr, const std::string &format) {
		this->value = tools::stringToTime(timeStr, format);
	}
	bool TimeValue::isStringLike() {
		return true;
	}

	DateTimeValue::DateTimeValue(const std::chrono::system_clock::time_point &value):
		SqlValue(SqlType::DateTime), value(value) {}
	DateTimeValue::DateTimeValue(const std::string &dateTimeStr, const std::string &format):
		SqlValue(SqlType::DateTime), value(tools::stringToDateTime(dateTimeStr, format)) {}
	std::string DateTimeValue::toString() const {
		return tools::dateTimeToString(value);
	}
	std::string DateTimeValue::toString(const std::string &format) const {
		return tools::dateTimeToString(value, format);
	}
	std::chrono::system_clock::time_point* DateTimeValue::getPtr() {
		return &value;
	}
	void DateTimeValue::setValue(const std::chrono::system_clock::time_point &value) {
		this->value = value;
	}
	void DateTimeValue::setValue(const std::string &dateTimeStr, const std::string &format) {
		this->value = tools::stringToDateTime(dateTimeStr, format);
	}
	bool DateTimeValue::isStringLike() {
		return true;
	}

	TimestampValue::TimestampValue(const std::chrono::system_clock::time_point &value):
		SqlValue(SqlType::Timestamp), value(value) {}
	TimestampValue::TimestampValue(const std::string &timestampStr, const std::string &format):
		SqlValue(SqlType::Timestamp), value(tools::stringToTimestamp(timestampStr, format)) {}
	std::string TimestampValue::toString() const {
		return tools::timestampToString(value);
	}
	std::string TimestampValue::toString(const std::string &format) const {
		return tools::timestampToString(value, format);
	}
	std::chrono::system_clock::time_point* TimestampValue::getPtr() {
		return &value;
	}
	void TimestampValue::setValue(const std::chrono::system_clock::time_point &value) {
		this->value = value;
	}
	void TimestampValue::setValue(const std::string &timestampStr, const std::string &format) {
		this->value = tools::stringToTimestamp(timestampStr, format);
	}
	bool TimestampValue::isStringLike() {
		return true;
	}

	BooleanValue::BooleanValue(const bool value):
		SqlValue(SqlType::Boolean), value(value) {}
	BooleanValue::BooleanValue(const int value):
		SqlValue(SqlType::Boolean), value(value != 0) {}
	std::string BooleanValue::toString() const {
		return value ? "true" : "false";
	}
	bool* BooleanValue::getPtr() {
		return &value;
	}
	void BooleanValue::setValue(const bool value) {
		this->value = value;
	}
	void BooleanValue::setValue(const int value) {
		this->value = value != 0;
	}

}

namespace condition {

	CompositeCondition::CompositeCondition(std::unique_ptr<ConditionBase> left, std::unique_ptr<ConditionBase> right, const SqlOperator op):
		left(std::move(left)), right(std::move(right)), op(op) {}
	std::string CompositeCondition::toSql() const {
		std::ostringstream result{};
		const std::string opStr = SqlOperatorDef[op];
		if (op == SqlOperator::NOT) {
			auto leftSql = left->toSql();
			result << opStr;
			if (leftSql.front() == '(' and leftSql.back() == ')')
				result << leftSql;
			else
			result << "(" << left->toSql() << ")";
		}
		else
			result << "(" << left->toSql() << " " << opStr << " " << right->toSql() << ")";
		return result.str();
	}
	std::string CompositeCondition::toString() const { return toSql(); }
	std::unique_ptr<ConditionBase> CompositeCondition::clone() const {
		return std::make_unique<CompositeCondition>(
			left ? left->clone() : nullptr,
			right ? right->clone() : nullptr,
			op
		);
	}
	CompositeCondition::operator std::string() const { return toString(); }

}

namespace column {

	ColumnBase::ColumnBase(std::string name, const value::SqlType type, std::string tableName, const bool primaryKey, const bool nullable):
		name(std::move(name)), tableName(std::move(tableName)), type(type), primaryKey(primaryKey), nullable(nullable) {}
	std::string ColumnBase::getName() const {
		return name;
	}
	std::string ColumnBase::getTableName() const {
		return tableName;
	}
	std::string ColumnBase::getFullName() const {
		return tableName.empty() ? name : tableName + "." + name;
	}
	value::SqlType ColumnBase::getType() const {
		return type;
	}
	std::string ColumnBase::getTypeDefinition() const {
		return value::SqlTypeDef[type];
	}
	bool ColumnBase::isPrimaryKey() const {
		return primaryKey;
	}
	bool ColumnBase::isNullable() const {
		return nullable;
	}
	std::string ColumnBase::toString() const {
		return name + " " + getTypeDefinition() + (isPrimaryKey() ? " PRIMARY KEY" : "") + (isNullable() ? " NULL" : " NOT NULL");
	}
	void ColumnBase::setTableName(const std::string& tableName) {
		this->tableName = tableName;
	}
	bool ColumnBase::isPtrNull() const {
		return getPtr() == nullptr;
	}
	bool ColumnBase::isPtrNotNull() const {
		return getPtr() != nullptr;
	}
	bool ColumnBase::isStringLike() {
		return false;
	}
	condition::Condition<std::string> ColumnBase::isNull() const {
		return {getFullName(), condition::SqlOperator::IS_NULL, ""};
	}
	condition::Condition<std::string> ColumnBase::isNotNull() const {
		return {getFullName(), condition::SqlOperator::IS_NOT_NULL, ""};
	}
	ColumnBase::operator std::string() const {
		return toString();
	}
	void ColumnBase::setDirty(const bool dirty) {
		this->dirty = dirty;
	}
	bool ColumnBase::getDirty() const {
		return dirty;
	}
	void ColumnBase::markDirty() {
		setDirty(true);
	}
	void ColumnBase::clearDirty() {
		setDirty(false);
	}

	SerialColumn::SerialColumn(const std::string& name, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Serial, "", primaryKey, nullable) {}
	SerialColumn::SerialColumn(const std::string& name, const unsigned int value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Serial, "", primaryKey, false) {
		this->setValue(value);
	}
	SerialColumn::SerialColumn(const SerialColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	SerialColumn::~SerialColumn() {
		if (value)
			this->value.reset();
	}
	unsigned int* SerialColumn::getPtrValue() const {
		return static_cast<unsigned int*>(getPtr());
	}
	std::string SerialColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue())
			return std::to_string(*ptr);
		return "NULL";
	}
	void SerialColumn::setValue(const unsigned int value) {
		if (not this->value)
			this->value = std::make_unique<value::SerialValue>(value);
		else
			this->value->setValue(value);
	}
	void SerialColumn::setNull() {
		throw std::runtime_error(toString() + " cannot be null");
	}
	void SerialColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			throw std::runtime_error(toString() + " cannot be null");
		value = std::make_unique<value::SerialValue>(*static_cast<const unsigned int*>(ptr));
	}
	void SerialColumn::setValueFromField(const pqxx::field& field) {
		value = std::make_unique<value::SerialValue>(field.as<unsigned int>());
	}
	void* SerialColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	SerialColumn& SerialColumn::operator = (const unsigned int value) {
		setValue(value);
		return *this;
	}
	std::unique_ptr<ColumnBase> SerialColumn::clone() const {
		return std::make_unique<SerialColumn>(*this);
	}
	condition::Condition<unsigned int> SerialColumn::operator == (const unsigned int value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<unsigned int> SerialColumn::operator != (const unsigned int value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<unsigned int> SerialColumn::operator < (const unsigned int value) const {
		return {getFullName(), condition::SqlOperator::LESS, value};
	}
	condition::Condition<unsigned int> SerialColumn::operator <= (const unsigned int value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, value};
	}
	condition::Condition<unsigned int> SerialColumn::operator > (const unsigned int value) const {
		return {getFullName(), condition::SqlOperator::GREATER, value};
	}
	condition::Condition<unsigned int> SerialColumn::operator >= (const unsigned int value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, value};
	}
	condition::Condition<unsigned int> SerialColumn::in(const std::vector<unsigned int>& values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<unsigned int> SerialColumn::notIn(const std::vector<unsigned int>& values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}

	IntegerColumn::IntegerColumn(const std::string &name, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Integer, "", primaryKey, nullable) {}
	IntegerColumn::IntegerColumn(const std::string &name, const int value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Integer, "", primaryKey, nullable) {
		this->setValue(value);
	}
	IntegerColumn::IntegerColumn(const IntegerColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	IntegerColumn::~IntegerColumn() {
		if (value)
			this->value.reset();
	}
	int* IntegerColumn::getPtrValue() const {
		return static_cast<int*>(getPtr());
	}
	std::string IntegerColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue())
			return std::to_string(*ptr);
		return "NULL";
	}
	void IntegerColumn::setValue(const int value) {
		if (not this->value)
			this->value = std::make_unique<value::IntegerValue>(value);
		else
			this->value->setValue(value);
	}
	void IntegerColumn::setNull() {
		if (not isNullable())
			throw std::runtime_error(toString() + " is not nullable");
		this->value.reset();
	}
	void IntegerColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else
			value = std::make_unique<value::IntegerValue>(*static_cast<const int*>(ptr));
	}
	void IntegerColumn::setValueFromField(const pqxx::field& field) {
		if (field.is_null())
			setNull();
		else
			value = std::make_unique<value::IntegerValue>(field.as<int>());
	}
	void* IntegerColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	std::unique_ptr<ColumnBase> IntegerColumn::clone() const {
		return std::make_unique<IntegerColumn>(*this);
	}
	IntegerColumn& IntegerColumn::operator = (const int value) {
		setValue(value);
		return *this;
	}
	condition::Condition<int> IntegerColumn::operator == (const int value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<int> IntegerColumn::operator != (const int value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<int> IntegerColumn::operator < (const int value) const {
		return {getFullName(), condition::SqlOperator::LESS, value};
	}
	condition::Condition<int> IntegerColumn::operator <= (const int value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, value};
	}
	condition::Condition<int> IntegerColumn::operator > (const int value) const {
		return {getFullName(), condition::SqlOperator::GREATER, value};
	}
	condition::Condition<int> IntegerColumn::operator >= (const int value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, value};
	}
	condition::Condition<int> IntegerColumn::in(const std::vector<int>& values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<int> IntegerColumn::notIn(const std::vector<int>& values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}

	StringColumn::StringColumn(const std::string &name, const size_t length, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::String, "", primaryKey, nullable), length(length) {}
	StringColumn::StringColumn(const std::string &name, const std::string& value, const size_t length, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::String, "", primaryKey, nullable), length(length) {
		this->setValue(value);
	}
	StringColumn::StringColumn(const StringColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()), length(other.getLength()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	StringColumn::~StringColumn() {
		if (value)
			this->value.reset();
	}
	std::string * StringColumn::getPtrValue() const {
		return static_cast<std::string*>(getPtr());
	}
	size_t StringColumn::getLength() const {
		return length;
	}
	std::string StringColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue()) {
			return tools::format(*ptr, true);
		}
		return tools::format("", false, tools::SqlSpecialValue::NULL_VALUE);
	}
	void StringColumn::setValue(const std::string& value) {
		if (value.length() > length)
			throw std::invalid_argument("Value too long");
		if (not this->value)
			this->value = std::make_unique<value::StringValue>(value);
		else
			this->value->setValue(value);
	}
	void StringColumn::setNull() {
		if (not isNullable())
			throw std::runtime_error(toString() + " is not nullable");
		this->value.reset();
	}
	void StringColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else {
			const auto val = *static_cast<const std::string*>(ptr);
			if (val.length() > length)
				throw std::invalid_argument("Value too long");
			value = std::make_unique<value::StringValue>(val);
		}
	}
	void StringColumn::setValueFromField(const pqxx::field& field) {
		if (field.is_null())
			setNull();
		else {
			const auto val = field.as<std::string>();
			if (val.length() > length)
				throw std::invalid_argument("Value too long");
			value = std::make_unique<value::StringValue>(val);
		}
	}
	void* StringColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	std::unique_ptr<ColumnBase> StringColumn::clone() const {
		return std::make_unique<StringColumn>(*this);
	}
	bool StringColumn::isStringLike() {
		return false;
	}
	StringColumn& StringColumn::operator = (const std::string& value) {
		setValue(value);
		return *this;
	}
	StringColumn& StringColumn::operator = (const char* value) {
		setValue(std::string(value));;
		return *this;
	}
	condition::Condition<std::string> StringColumn::operator == (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<std::string> StringColumn::operator != (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<std::string> StringColumn::operator < (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS, value};
	}
	condition::Condition<std::string> StringColumn::operator <= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, value};
	}
	condition::Condition<std::string> StringColumn::operator > (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, value};
	}
	condition::Condition<std::string> StringColumn::operator >= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, value};
	}
	condition::Condition<std::string> StringColumn::in(const std::vector<std::string>& values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> StringColumn::notIn(const std::vector<std::string>& values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> StringColumn::like(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> StringColumn::notLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}
	condition::Condition<std::string> StringColumn::iLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::ILIKE, value};
	}
	condition::Condition<std::string> StringColumn::notILike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_ILIKE, value};
	}

	TextColumn::TextColumn(const std::string &name, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Text, "", primaryKey, nullable) {}
	TextColumn::TextColumn(const std::string &name, const std::string &value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Text, "", primaryKey, nullable) {
		this->setValue(value);
	}
	TextColumn::TextColumn(const TextColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	TextColumn::~TextColumn() {
		if (value)
			this->value.reset();
	}
	std::string* TextColumn::getPtrValue() const {
		return static_cast<std::string*>(getPtr());
	}
	void TextColumn::setValue(const std::string &value) {
		if (not this->value)
			this->value = std::make_unique<value::TextValue>(value);
		else
			this->value->setValue(value);
	}
	std::string TextColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue()) {
			return tools::format(*ptr, true);
		}
		return tools::format("", false, tools::SqlSpecialValue::NULL_VALUE);
	}
	void TextColumn::setNull() {
		if (not isNullable())
			throw std::runtime_error(toString() + " is not nullable");
		this->value.reset();
	}
	void TextColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else
			value = std::make_unique<value::TextValue>(*static_cast<const std::string*>(ptr));
	}
	void TextColumn::setValueFromField(const pqxx::field& field) {
		if (field.is_null())
			setNull();
		else
			value = std::make_unique<value::TextValue>(field.as<std::string>());
	}
	void* TextColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	std::unique_ptr<ColumnBase> TextColumn::clone() const {
		return std::make_unique<TextColumn>(*this);
	}
	bool TextColumn::isStringLike() {
		return false;
	}
	TextColumn& TextColumn::operator = (const std::string& value) {
		setValue(value);
		return *this;
	}
	TextColumn& TextColumn::operator = (const char* value) {
		setValue(std::string(value));
		return *this;
	}
	condition::Condition<std::string> TextColumn::operator == (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<std::string> TextColumn::operator != (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<std::string> TextColumn::operator < (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS, value};
	}
	condition::Condition<std::string> TextColumn::operator <= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, value};
	}
	condition::Condition<std::string> TextColumn::operator > (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, value};
	}
	condition::Condition<std::string> TextColumn::operator >= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, value};
	}
	condition::Condition<std::string> TextColumn::in(const std::vector<std::string>& values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> TextColumn::notIn(const std::vector<std::string>& values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> TextColumn::like(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> TextColumn::notLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}
	condition::Condition<std::string> TextColumn::iLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::ILIKE, value};
	}
	condition::Condition<std::string> TextColumn::notILike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_ILIKE, value};
	}

	DecimalColumn::DecimalColumn(const std::string &name, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Decimal, "", primaryKey, nullable) {}
	DecimalColumn::DecimalColumn(const std::string &name, const double value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Decimal, "", primaryKey, nullable) {
		this->setValue(value);
	}
	DecimalColumn::DecimalColumn(const DecimalColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	DecimalColumn::~DecimalColumn() {
		if (value)
			this->value.reset();
	}
	double *DecimalColumn::getPtrValue() const {
		return static_cast<double*>(getPtr());
	}
	std::string DecimalColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue())
			return std::to_string(*ptr);
		return "NULL";
	}
	void DecimalColumn::setValue(const double value) {
		if (not this->value)
			this->value = std::make_unique<value::DecimalValue>(value);
		else
			this->value->setValue(value);
	}
	void DecimalColumn::setNull() {
		if (not isNullable())
			throw std::runtime_error(toString() + " is not nullable");
		this->value.reset();
	}
	void DecimalColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else
			value = std::make_unique<value::DecimalValue>(*static_cast<const double*>(ptr));
	}
	void DecimalColumn::setValueFromField(const pqxx::field& field) {
		if (field.is_null())
			setNull();
		else
			value = std::make_unique<value::DecimalValue>(field.as<double>());
	}
	void* DecimalColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	std::unique_ptr<ColumnBase> DecimalColumn::clone() const {
		return std::make_unique<DecimalColumn>(*this);
	}
	DecimalColumn& DecimalColumn::operator = (const double value) {
		setValue(value);
		return *this;
	}
	condition::Condition<double> DecimalColumn::operator == (const double value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<double> DecimalColumn::operator != (const double value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<double> DecimalColumn::operator < (const double value) const {
		return {getFullName(), condition::SqlOperator::LESS, value};
	}
	condition::Condition<double> DecimalColumn::operator <= (const double value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, value};
	}
	condition::Condition<double> DecimalColumn::operator > (const double value) const {
		return {getFullName(), condition::SqlOperator::GREATER, value};
	}
	condition::Condition<double> DecimalColumn::operator >= (const double value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, value};
	}
	condition::Condition<double> DecimalColumn::in(const std::vector<double>& values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<double> DecimalColumn::notIn(const std::vector<double>& values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}

	DateColumn::DateColumn(const std::string &name, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Date, "", primaryKey, nullable) {}
	DateColumn::DateColumn(const std::string &name, const std::chrono::system_clock::time_point &value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Date, "", primaryKey, nullable) {
		this->setValue(value);
	}
	DateColumn::DateColumn(const std::string &name, const std::string &dateStr, const std::string &format, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Date, "", primaryKey, nullable) {
		this->setValue(dateStr, format);
	}
	DateColumn::DateColumn(const DateColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	DateColumn::~DateColumn() {
		if (value)
			this->value.reset();
	}
	std::chrono::system_clock::time_point *DateColumn::getPtrValue() const {
		return static_cast<std::chrono::system_clock::time_point*>(getPtr());
	}
	std::string DateColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue()) {
			return tools::format(tools::dateToString(*ptr), true);
		}
		return tools::format("", false, isNullable() ? tools::SqlSpecialValue::NULL_VALUE : tools::SqlSpecialValue::CURRENT_DATE);
	}
	void DateColumn::setValue(const std::chrono::system_clock::time_point &value) {
		if (not this->value)
			this->value = std::make_unique<value::DateValue>(value);
		else
			this->value->setValue(value);
	}
	void DateColumn::setValue(const std::string &dateStr, const std::string &format) {
		if (not this->value)
			this->value = std::make_unique<value::DateValue>(dateStr, format);
		else
			this->value->setValue(dateStr, format);
	}
	void DateColumn::setNull() {
		if (not isNullable())
			throw std::runtime_error(toString() + " is not nullable");
		this->value.reset();
	}
	void DateColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else
			value = std::make_unique<value::DateValue>(*static_cast<const std::chrono::system_clock::time_point*>(ptr));
	}
	void DateColumn::setValueFromField(const pqxx::field& field) {
		if (field.is_null())
			setNull();
		else
			value = std::make_unique<value::DateValue>(field.as<std::string>());
	}
	void* DateColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	std::unique_ptr<ColumnBase> DateColumn::clone() const {
		return std::make_unique<DateColumn>(*this);
	}
	bool DateColumn::isStringLike() {
		return false;
	}
	DateColumn& DateColumn::operator = (const std::chrono::system_clock::time_point &value) {
		setValue(value);
		return *this;
	}
	DateColumn& DateColumn::operator = (const std::string& dateStr) {
		setValue(dateStr);
		return *this;
	}
	DateColumn& DateColumn::operator = (const char* dateStr) {
		setValue(std::string(dateStr));
		return *this;
	}
	condition::Condition<std::string> DateColumn::operator == (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<std::string> DateColumn::operator == (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, tools::dateToString(value)};
	}
	condition::Condition<std::string> DateColumn::operator != (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<std::string> DateColumn::operator != (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, tools::dateToString(value)};
	}
	condition::Condition<std::string> DateColumn::operator < (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS, value};
	}
	condition::Condition<std::string> DateColumn::operator < (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::LESS, tools::dateToString(value)};
	}
	condition::Condition<std::string> DateColumn::operator <= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, value};
	}
	condition::Condition<std::string> DateColumn::operator <= (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, tools::dateToString(value)};
	}
	condition::Condition<std::string> DateColumn::operator > (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, value};
	}
	condition::Condition<std::string> DateColumn::operator > (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, tools::dateToString(value)};
	}
	condition::Condition<std::string> DateColumn::operator >= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, value};
	}
	condition::Condition<std::string> DateColumn::operator >= (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, tools::dateToString(value)};
	}
	condition::Condition<std::string> DateColumn::in(const std::vector<std::string> &values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> DateColumn::in(const std::vector<std::chrono::system_clock::time_point> &value) const {
		std::vector<std::string> values;
		values.reserve(value.size());
		for (const auto &v : value)
			values.push_back(tools::dateToString(v));
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> DateColumn::notIn(const std::vector<std::string> &values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> DateColumn::notIn(const std::vector<std::chrono::system_clock::time_point> &value) const {
		std::vector<std::string> values;
		values.reserve(value.size());
		for (const auto &v : value)
			values.push_back(tools::dateToString(v));
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> DateColumn::like(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> DateColumn::notLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}
	condition::Condition<std::string> DateColumn::iLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> DateColumn::notILike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}

	TimeColumn::TimeColumn(const std::string &name, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Time, "", primaryKey, nullable) {}
	TimeColumn::TimeColumn(const std::string &name, const std::chrono::seconds &value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Time, "", primaryKey, nullable) {
		this->setValue(value);
	}
	TimeColumn::TimeColumn(const std::string &name, const std::string &timeStr, const std::string &format, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Time, "", primaryKey, nullable) {
		this->setValue(timeStr, format);
	}
	TimeColumn::TimeColumn(const TimeColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	TimeColumn::~TimeColumn() {
		if (value)
			this->value.reset();
	}
	std::chrono::seconds *TimeColumn::getPtrValue() const {
		return static_cast<std::chrono::seconds*>(getPtr());
	}
	std::string TimeColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue()) {
			return tools::format(tools::timeToString(*ptr), true);
		}
		return tools::format("", false, isNullable() ? tools::SqlSpecialValue::NULL_VALUE : tools::SqlSpecialValue::CURRENT_TIME);
	}
	void TimeColumn::setValue(const std::chrono::seconds &value) {
		if (not this->value)
			this->value = std::make_unique<value::TimeValue>(value);
		else
			this->value->setValue(value);
	}
	void TimeColumn::setValue(const std::string &timeStr, const std::string &format) {
		if (not this->value)
			this->value = std::make_unique<value::TimeValue>(timeStr, format);
		else
			this->value->setValue(timeStr, format);
	}
	void TimeColumn::setNull() {
		if (not isNullable())
			throw std::runtime_error(toString() + " is not nullable");
		this->value.reset();
	}
	void TimeColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else
			value = std::make_unique<value::TimeValue>(*static_cast<const std::chrono::seconds*>(ptr));
	}
	void TimeColumn::setValueFromField(const pqxx::field& field) {
		if (field.is_null())
			setNull();
		else
			value = std::make_unique<value::TimeValue>(field.as<std::string>());
	}
	void* TimeColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	std::unique_ptr<ColumnBase> TimeColumn::clone() const {
		return std::make_unique<TimeColumn>(*this);
	}
	bool TimeColumn::isStringLike() {
		return false;
	}
	TimeColumn& TimeColumn::operator = (const std::chrono::seconds &value) {
		setValue(value);
		return *this;
	}
	TimeColumn& TimeColumn::operator = (const std::string& timeStr) {
		setValue(timeStr);
		return *this;
	}
	TimeColumn& TimeColumn::operator = (const char* timeStr) {
		setValue(std::string(timeStr));
		return *this;
	}
	condition::Condition<std::string> TimeColumn::operator == (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<std::string> TimeColumn::operator == (const std::chrono::seconds& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, tools::timeToString(value)};
	}
	condition::Condition<std::string> TimeColumn::operator != (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<std::string> TimeColumn::operator != (const std::chrono::seconds& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, tools::timeToString(value)};
	}
	condition::Condition<std::string> TimeColumn::operator < (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS, value};
	}
	condition::Condition<std::string> TimeColumn::operator < (const std::chrono::seconds& value) const {
		return {getFullName(), condition::SqlOperator::LESS, tools::timeToString(value)};
	}
	condition::Condition<std::string> TimeColumn::operator <= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, value};
	}
	condition::Condition<std::string> TimeColumn::operator <= (const std::chrono::seconds& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, tools::timeToString(value)};
	}
	condition::Condition<std::string> TimeColumn::operator > (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, value};
	}
	condition::Condition<std::string> TimeColumn::operator > (const std::chrono::seconds& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, tools::timeToString(value)};
	}
	condition::Condition<std::string> TimeColumn::operator >= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, value};
	}
	condition::Condition<std::string> TimeColumn::operator >= (const std::chrono::seconds& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, tools::timeToString(value)};
	}
	condition::Condition<std::string> TimeColumn::in(const std::vector<std::string> &values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> TimeColumn::in(const std::vector<std::chrono::seconds> &value) const {
		std::vector<std::string> values;
		values.reserve(value.size());
		for (const auto &v : value)
			values.push_back(tools::timeToString(v));
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> TimeColumn::notIn(const std::vector<std::string> &values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> TimeColumn::notIn(const std::vector<std::chrono::seconds> &value) const {
		std::vector<std::string> values;
		values.reserve(value.size());
		for (const auto &v : value)
			values.push_back(tools::timeToString(v));
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> TimeColumn::like(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> TimeColumn::notLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}
	condition::Condition<std::string> TimeColumn::iLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> TimeColumn::notILike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}

	DateTimeColumn::DateTimeColumn(const std::string &name, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::DateTime, "", primaryKey, nullable) {}
	DateTimeColumn::DateTimeColumn(const std::string &name, const std::chrono::system_clock::time_point &value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::DateTime, "", primaryKey, nullable) {
		this->setValue(value);
	}
	DateTimeColumn::DateTimeColumn(const std::string &name, const std::string &dateTimeStr, const std::string &format, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::DateTime, "", primaryKey, nullable) {
		this->setValue(dateTimeStr, format);
	}
	DateTimeColumn::DateTimeColumn(const DateTimeColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	DateTimeColumn::~DateTimeColumn() {
		if (value)
			this->value.reset();
	}
	std::chrono::system_clock::time_point *DateTimeColumn::getPtrValue() const {
		return static_cast<std::chrono::time_point<std::chrono::system_clock>*>(getPtr());
	}
	std::string DateTimeColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue()) {
			return tools::format(tools::dateTimeToString(*ptr), true);
		}
		return tools::format("", false, isNullable() ? tools::SqlSpecialValue::NULL_VALUE : tools::SqlSpecialValue::CURRENT_TIMESTAMP);
	}
	void DateTimeColumn::setValue(const std::chrono::system_clock::time_point &value) {
		if (not this->value)
			this->value = std::make_unique<value::DateTimeValue>(value);
		else
			this->value->setValue(value);
	}
	void DateTimeColumn::setValue(const std::string &dateTimeStr, const std::string &format) {
		if (not this->value)
			this->value = std::make_unique<value::DateTimeValue>(dateTimeStr, format);
		else
			this->value->setValue(dateTimeStr, format);
	}
	void DateTimeColumn::setNull() {
		if (not isNullable())
			throw std::runtime_error(toString() + " is not nullable");
		this->value.reset();
	}
	void DateTimeColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else
			value = std::make_unique<value::DateTimeValue>(*static_cast<const std::chrono::system_clock::time_point*>(ptr));
	}
	void DateTimeColumn::setValueFromField(const pqxx::field& field) {
		if (field.is_null())
			setNull();
		else
			value = std::make_unique<value::DateTimeValue>(field.as<std::string>());
	}
	void* DateTimeColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	std::unique_ptr<ColumnBase> DateTimeColumn::clone() const {
		return std::make_unique<DateTimeColumn>(*this);
	}
	bool DateTimeColumn::isStringLike() {
		return false;
	}
	DateTimeColumn& DateTimeColumn::operator = (const std::chrono::system_clock::time_point &value) {
		setValue(value);
		return *this;
	}
	DateTimeColumn& DateTimeColumn::operator = (const std::string& dateTimeStr) {
		setValue(dateTimeStr);
		return *this;
	}
	DateTimeColumn& DateTimeColumn::operator = (const char* dateTimeStr) {
		setValue(std::string(dateTimeStr));
		return *this;
	}
	condition::Condition<std::string> DateTimeColumn::operator == (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<std::string> DateTimeColumn::operator == (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, tools::dateTimeToString(value)};
	}
	condition::Condition<std::string> DateTimeColumn::operator != (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<std::string> DateTimeColumn::operator != (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, tools::dateTimeToString(value)};
	}
	condition::Condition<std::string> DateTimeColumn::operator < (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS, value};
	}
	condition::Condition<std::string> DateTimeColumn::operator < (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::LESS, tools::dateTimeToString(value)};
	}
	condition::Condition<std::string> DateTimeColumn::operator <= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, value};
	}
	condition::Condition<std::string> DateTimeColumn::operator <= (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, tools::dateTimeToString(value)};
	}
	condition::Condition<std::string> DateTimeColumn::operator > (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, value};
	}
	condition::Condition<std::string> DateTimeColumn::operator > (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, tools::dateTimeToString(value)};
	}
	condition::Condition<std::string> DateTimeColumn::operator >= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, value};
	}
	condition::Condition<std::string> DateTimeColumn::operator >= (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, tools::dateTimeToString(value)};
	}
	condition::Condition<std::string> DateTimeColumn::in(const std::vector<std::string> &values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> DateTimeColumn::in(const std::vector<std::chrono::system_clock::time_point> &value) const {
		std::vector<std::string> values;
		values.reserve(value.size());
		for (const auto &v : value)
			values.push_back(tools::dateTimeToString(v));
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> DateTimeColumn::notIn(const std::vector<std::string> &values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> DateTimeColumn::notIn(const std::vector<std::chrono::system_clock::time_point> &value) const {
		std::vector<std::string> values;
		values.reserve(value.size());
		for (const auto &v : value)
			values.push_back(tools::dateTimeToString(v));
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> DateTimeColumn::like(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> DateTimeColumn::notLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}
	condition::Condition<std::string> DateTimeColumn::iLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> DateTimeColumn::notILike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}

	TimestampColumn::TimestampColumn(const std::string &name, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Timestamp, "", primaryKey, nullable) {}
	TimestampColumn::TimestampColumn(const std::string &name, const std::chrono::system_clock::time_point &value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Timestamp, "", primaryKey, nullable) {
		this->setValue(value);
	}
	TimestampColumn::TimestampColumn(const std::string &name, const std::string &dateTimeStr, const std::string &format, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Timestamp, "", primaryKey, nullable) {
		this->setValue(dateTimeStr, format);
	}
	TimestampColumn::TimestampColumn(const TimestampColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	TimestampColumn::~TimestampColumn() {
		if (value)
			this->value.reset();
	}
	std::chrono::system_clock::time_point *TimestampColumn::getPtrValue() const {
		return static_cast<std::chrono::time_point<std::chrono::system_clock>*>(getPtr());
	}
	std::string TimestampColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue()) {
			return tools::format(tools::timestampToString(*ptr), true);
		}
		return tools::format("", false, isNullable() ? tools::SqlSpecialValue::NULL_VALUE : tools::SqlSpecialValue::CURRENT_TIMESTAMP);
	}
	void TimestampColumn::setValue(const std::chrono::system_clock::time_point &value) {
		if (not this->value)
			this->value = std::make_unique<value::TimestampValue>(value);
		else
			this->value->setValue(value);
	}
	void TimestampColumn::setValue(const std::string &timestampStr, const std::string &format) {
		if (not this->value)
			this->value = std::make_unique<value::TimestampValue>(timestampStr, format);
		else
			this->value->setValue(timestampStr, format);
	}
	void TimestampColumn::setNull() {
		if (not isNullable())
			throw std::runtime_error(toString() + " is not nullable");
		this->value.reset();
	}
	void TimestampColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else
			value = std::make_unique<value::TimestampValue>(*static_cast<const std::chrono::system_clock::time_point*>(ptr));
	}
	void TimestampColumn::setValueFromField(const pqxx::field& field) {
		if (field.is_null())
			setNull();
		else
			value = std::make_unique<value::TimestampValue>(field.as<std::string>());
	}
	void* TimestampColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	std::unique_ptr<ColumnBase> TimestampColumn::clone() const {
		return std::make_unique<TimestampColumn>(*this);
	}
	bool TimestampColumn::isStringLike() {
		return false;
	}
	TimestampColumn& TimestampColumn::operator = (const std::chrono::system_clock::time_point &value) {
		setValue(value);
		return *this;
	}
	TimestampColumn& TimestampColumn::operator = (const std::string& timestampStr) {
		setValue(timestampStr);
		return *this;
	}
	TimestampColumn& TimestampColumn::operator = (const char* timestampStr) {
		setValue(std::string(timestampStr));
		return *this;
	}
	condition::Condition<std::string> TimestampColumn::operator == (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<std::string> TimestampColumn::operator == (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, tools::timestampToString(value)};
	}
	condition::Condition<std::string> TimestampColumn::operator != (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<std::string> TimestampColumn::operator != (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, tools::timestampToString(value)};
	}
	condition::Condition<std::string> TimestampColumn::operator < (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS, value};
	}
	condition::Condition<std::string> TimestampColumn::operator < (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::LESS, tools::timestampToString(value)};
	}
	condition::Condition<std::string> TimestampColumn::operator <= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, value};
	}
	condition::Condition<std::string> TimestampColumn::operator <= (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::LESS_EQUAL, tools::timestampToString(value)};
	}
	condition::Condition<std::string> TimestampColumn::operator > (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, value};
	}
	condition::Condition<std::string> TimestampColumn::operator > (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::GREATER, tools::timestampToString(value)};
	}
	condition::Condition<std::string> TimestampColumn::operator >= (const std::string& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, value};
	}
	condition::Condition<std::string> TimestampColumn::operator >= (const std::chrono::system_clock::time_point& value) const {
		return {getFullName(), condition::SqlOperator::GREATER_EQUAL, tools::timestampToString(value)};
	}
	condition::Condition<std::string> TimestampColumn::in(const std::vector<std::string> &values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> TimestampColumn::in(const std::vector<std::chrono::system_clock::time_point> &value) const {
		std::vector<std::string> values;
		values.reserve(value.size());
		for (const auto &v : value)
			values.push_back(tools::timestampToString(v));
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<std::string> TimestampColumn::notIn(const std::vector<std::string> &values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> TimestampColumn::notIn(const std::vector<std::chrono::system_clock::time_point> &value) const {
		std::vector<std::string> values;
		values.reserve(value.size());
		for (const auto &v : value)
			values.push_back(tools::timestampToString(v));
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<std::string> TimestampColumn::like(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> TimestampColumn::notLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}
	condition::Condition<std::string> TimestampColumn::iLike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::LIKE, value};
	}
	condition::Condition<std::string> TimestampColumn::notILike(const std::string& value) const {
		return {getFullName(), condition::SqlOperator::NOT_LIKE, value};
	}

	BooleanColumn::BooleanColumn(const std::string &name, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Boolean, "", primaryKey, nullable) {}
	BooleanColumn::BooleanColumn(const std::string &name, const bool value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Boolean, "", primaryKey, nullable) {
		this->setValue(value);
	}
	BooleanColumn::BooleanColumn(const std::string &name, const int value, const bool primaryKey, const bool nullable):
		ColumnBase(name, value::SqlType::Boolean, "", primaryKey, nullable) {
		this->setValue(value);
	}
	BooleanColumn::BooleanColumn(const BooleanColumn& other):
		ColumnBase(other.getName(), other.getType(), other.getTableName(), other.isPrimaryKey(), other.isNullable()) {
		if (other.value)
			setValue(*other.value->getPtr());
	}
	BooleanColumn::~BooleanColumn() { if (value) this->value.reset(); }
	bool *BooleanColumn::getPtrValue() const {
		return static_cast<bool*>(getPtr());
	}
	std::string BooleanColumn::getValueAsString() const {
		if (const auto ptr = getPtrValue())
			return *ptr ? "true" : "false";
		return "NULL";
	}
	void BooleanColumn::setValue(const bool value) {
		if (not this->value)
			this->value = std::make_unique<value::BooleanValue>(value);
		else
			this->value->setValue(value);
	}
	void BooleanColumn::setValue(const int value) {
		if (not this->value)
			this->value = std::make_unique<value::BooleanValue>(value);
		else
			this->value->setValue(value);
	}
	void BooleanColumn::setNull() {
		if (not isNullable())
			throw std::runtime_error(toString() + " is not nullable");
		this->value.reset();
	}
	void BooleanColumn::setValueFromPtr(const void* ptr) {
		if (ptr == nullptr)
			setNull();
		else
			value = std::make_unique<value::BooleanValue>(*static_cast<const bool*>(ptr));
	}
	void BooleanColumn::setValueFromField(const pqxx::field& field) {
		if (field.is_null())
			setNull();
		else
			value = std::make_unique<value::BooleanValue>(field.as<bool>());
	}
	void* BooleanColumn::getPtr() const {
		return value ? value->getPtr() : nullptr;
	}
	std::unique_ptr<ColumnBase> BooleanColumn::clone() const {
		return std::make_unique<BooleanColumn>(*this);
	}
	BooleanColumn& BooleanColumn::operator = (const bool value) {
		setValue(value);
		return *this;
	}
	BooleanColumn& BooleanColumn::operator = (const int value) {
		setValue(value);
		return *this;
	}
	condition::Condition<bool> BooleanColumn::operator == (const bool value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value};
	}
	condition::Condition<bool> BooleanColumn::operator == (const int value) const {
		return {getFullName(), condition::SqlOperator::EQUAL, value != 0};
	}
	condition::Condition<bool> BooleanColumn::operator != (const bool value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value};
	}
	condition::Condition<bool> BooleanColumn::operator != (const int value) const {
		return {getFullName(), condition::SqlOperator::NOT_EQUAL, value != 0};
	}
	condition::Condition<bool> BooleanColumn::in(const std::vector<bool> &values) const {
		return {getFullName(), condition::SqlOperator::IN, values};
	}
	condition::Condition<bool> BooleanColumn::in(const std::vector<int> &values) const {
		std::vector<bool> boolValues;
		boolValues.reserve(values.size());
		for (const auto &v : values)
			boolValues.push_back(v != 0);
		return {getFullName(), condition::SqlOperator::IN, boolValues};
	}
	condition::Condition<bool> BooleanColumn::notIn(const std::vector<bool> &values) const {
		return {getFullName(), condition::SqlOperator::NOT_IN, values};
	}
	condition::Condition<bool> BooleanColumn::notIn(const std::vector<int> &values) const {
		std::vector<bool> boolValues;
		boolValues.reserve(values.size());
		for (const auto &v : values)
			boolValues.push_back(v != 0);
		return {getFullName(), condition::SqlOperator::NOT_IN, boolValues};
	}
	condition::Condition<bool> BooleanColumn::is(const bool value) const {
		const auto condition = value ? condition::SqlOperator::IS_TRUE : condition::SqlOperator::IS_FALSE;
		return {getFullName(), condition, value};
	}
	condition::Condition<bool> BooleanColumn::isNot(const bool value) const {
		const auto condition = value ? condition::SqlOperator::IS_TRUE : condition::SqlOperator::IS_FALSE;
		return {getFullName(), condition, value};
	}

}
