#pragma once
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>

namespace Asteroids
{

/*
Configuration files store variables for run-time loading.
In this project all configuration files have .txt extension,
and are placed in the Resources/Configurations/ directory.


The configuration file format permits the following format:

Line preceding with a "#" are comments. i.e:
# this is a comment
# this is another comment

Empty lines are permitted. i.e:
# this is a comment

# this is another comment

Variable declaration:
myVar		= 5
yourVar		= 10
# Notice how spaces and tabs are permitted
# between the variable name and the value.
# Also notice how there is only one space
# between the equals sign and value.

Bad variable declaration (will result in exception throw):
badVar randomChars123 = 7

Strings variables:
myString = This is my string
# Quotation marks are not needed.
# If they are added, they become part 
# of the string.
*/

class ConfigurationFile
{

public:

	ConfigurationFile(const std::string& fileName);
	~ConfigurationFile() = default;

	// Retrieves a property's value.
	// runtime_error is thrown if the property is badly formatted
	// or does not exist.
	template<typename T>
	T GetProperty(const std::string& property)
	{
		std::string propertyValueString = FindAndRetrievePropertyValueString(property);

		// Create a stream out of the propertyValueString so that we can
		// perform conversions from string to variable type T
		std::istringstream propertyValueStream(propertyValueString);

		T propertyValue;

		propertyValueStream >> propertyValue;

		return propertyValue;
	}

	// Specilization of GetProperty for strings.
	template<>
	std::string GetProperty<std::string>(const std::string& property)
	{
		// Since properties are found as strings, no conversion 
		// is required if the property type is also a string.
		return FindAndRetrievePropertyValueString(property);
	}

private:

	// After calling, the next read performed will be starting from the beginning.
	void ReturnToBeginningOfFile();

	// Determines whether line has zero-length.
	bool IsLineEmpty(const std::string& line) const;

	// Determines whether line is preceded by the comment marker.
	bool IsLineAComment(const std::string& line) const;

	// Determines whether line contains a property.
	bool DoesLineContainProperty(const std::string& line, const std::string& property) const;

	// Finds the property in the config file and returns its value as a string.
	std::string FindAndRetrievePropertyValueString(const std::string& property);

	std::fstream file;
	
	static const char CommentMarker{ '#' };
	static const std::string FileExtension;
	static const std::string FileDirectory;
};

} // namespace Asteroids