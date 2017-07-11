#include "ConfigurationFile.hpp"

namespace Asteroids
{

const std::string ConfigurationFile::FileExtension = ".txt";
const std::string ConfigurationFile::FileDirectory = "Resources/Configurations/";

ConfigurationFile::ConfigurationFile(const std::string& fileName)
{
	file.open(FileDirectory + fileName + FileExtension);

	if (file.fail())
	{
		throw std::runtime_error("Failed to open file " + fileName);
	}
}

void ConfigurationFile::ReturnToBeginningOfFile()
{
	// Clear any error flags that may have arose
	// and return the the beginning of the file
	file.clear();
	file.seekg(0, std::ios::beg);
}

bool ConfigurationFile::IsLineEmpty(const std::string& line) const
{
	return line.length() == 0;
}
	
bool ConfigurationFile::IsLineAComment(const std::string& line) const
{
	return (line[0] == CommentMarker);
}
	
bool ConfigurationFile::DoesLineContainProperty(const std::string& line, const std::string& property) const
{
	auto propertyPosition = line.find(property);

	// Property was not found on the line
	if (propertyPosition == property.npos)
	{
		return false;
	}

	// Property name appeared as a value.
	// i.e: bookTitle = propertyName
	//
	// In this scenario the name of the property we're looking for is in 
	// fact a value, and bookTitle is the property.
	if (propertyPosition != 0)
	{
		return false;
	}

	return true;
}

std::string ConfigurationFile::FindAndRetrievePropertyValueString(const std::string& property)
{
	ReturnToBeginningOfFile();

	std::string line;

	while (std::getline(file, line))
	{
		// Skip all lines which do not contain the property
		if (IsLineEmpty(line) || IsLineAComment(line) || !DoesLineContainProperty(line, property))
		{
			continue;
		}

		auto equalsPosition = line.find('=');

		// No equals sign found
		if (equalsPosition == line.npos)
		{
			throw std::runtime_error("Property: " + property + " is badly formatted");
		}

		// Between the property name and the equals signs there may only be 
		// spaces or tabs. So we begin at the end of the property name, and 
		// search for the first instance of no space or tab. If the line is 
		// formatted correctly, the first non space or tab will be the equals sign.
		auto nonSpaceOrTabPosition = line.find_first_not_of(" \t", property.length());

		// If the first non space or tab isn't the equals sign, there are 
		// invalid characters appearing between the property name and equals sign.
		// i.e: propertyNameInvalidChars123 = 17
		//                   ~~~~~~^~~~~~~
		if (nonSpaceOrTabPosition != equalsPosition)
		{
			throw std::runtime_error("Property: " + property + " is badly formatted");
		}

		// After the equals sign there must be one
		// space followed by the value (hence + 2). If the line
		// is too short to hold these, it is badly formatted.
		//
		// The line length is 1-indexed while the equals position
		// is 0-indexed. Thus 1 is subtracted from the line length
		// to avoid off-by-one error.
		if (line.length() - 1 < equalsPosition + 2)
		{
			throw std::runtime_error("Property: " + property + " is badly formatted");
		}

		// No space after equals sign
		if (line[equalsPosition + 1] != ' ')
		{
			throw std::runtime_error("Property: " + property + " is badly formatted");
		}

		// Everything after the "= " (equals and space is where the + 2 comes from)
		// is considered the value for the property
		std::string	propertyValueString = line.substr(equalsPosition + 2);

		return std::move(propertyValueString);
	}

	throw std::runtime_error("Failed to find property: " + property);
}

} // namespace Asteroids