/**
* File: iniParser.h
*
* Purpose: The header/interface of CIniParser Class. 
*
* 
* @author Zhang Yong
* @version 1.0 2015/02/04
*
* Copyright (C) 2009, 2015, iRay Technology (Shanghai) Ltd.
*
*/
#ifndef _INI_PARSER_H_
#define _INI_PARSER_H_


// C header files
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

// C++ header files
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <strstream>

using namespace std;

#define MAX_SECTION_NAME_LENGTH		    128
#define MAX_ITEM_NAME_LENGTH            128
#define MAX_ITEM_VALUE_LENGTH	        512

/**
* struct IniSection£ºstruct to hold ini Item/Value pair and comments
*
* @author Zhang Yong
* @version  1.0 2015/02/04
*
*/
typedef  struct _tagIniSection
{
	vector<string> vecItems;
	vector<string> vecValues; 
	vector<string> vecComments;
}IniSection;

/**
* class CIniParser£ºclass to manipulate the ini file content
*
* @author Zhang Yong
* @version  1.0 2015/02/04
*
*/

class CIniParser
{
public:
	/** Constructor*/
	CIniParser();

	/** Virtual Destructor*/
	virtual ~CIniParser();

	/**
	* ReadFile: Read ini file comments, sections, items and corresponding values into data structure stored in RAM
	* @param strPath: The path of the to be read out ini file 
	* @return value: Indicating whether reading ini file succeeded
	*/
	bool ReadFile(const string &strPath);

	/**
	* WriteFile: Write data structure in RAM to hard disk for permanent storage purpose
	* @param strPath: The path of the to be Written out ini file 
	* @return value: Indicating whether writing ini file succeeded
	*/
	bool WriteFile(const string &strPath); 

	/**
	* EraseAllContent: Erase all content stored in data structure in RAM
	* If later this data structure is saved to hard disk, then there will be no content on the saved file.
	* @return void
	*/
	void EraseAllContent();

	/**
	* GetSectionCount: Get section count in this ini file
	* @return value: The section total sum 
	*/
	unsigned GetSectionCount() const;

	/**
	* AddSection: Set section name, equivalent to adding read section in the data structure in RAM
	* @param strSectionName: Section name to be added to the data structure in RAM
	* @return value: The index of the newly added section 
	*/
	unsigned AddSection(const string &strSectionName);

	/**
	* GetItemCountInSection: Overloading function, getting item count in section specified by its name
	* @param nSectionName: Item count in the specified section would be retrieved
	* @return value: The item count in the specified section
	*/
	unsigned GetItemCountInSection(const string &strSectionName);

	/**
	* GetItemName: Overloading function, getting item name specified using section name and item Index
	* @param strSectionName: The section name whose item vector would be checked 
	* @param nItemIndex: The item index specified to retrieve its corresponding item name
	* @return value: The name of the specified item within a specified section
	*/
	string GetItemName(const string &strSectionName, unsigned nItemIndex) const;

	/**
	* GetItemValueS: Get item value in string format
	* @param strSectionName: The section name whose item vector would be checked 
	* @param strItemName: The item name specified to retrieve its corresponding item value
	* @return value: The value of the specified item within a specified section
	*/
	bool  GetItemValueS(const string &strSectionName, const string &strItemName, string &strValue) const; 

	/**
	* GetItemValueI: Get item value in Integer format
	* @param strSectionName: The section name whose item vector would be checked 
	* @param strItemName: The item name specified to retrieve its corresponding item value
	* @return value: The value of the specified item within a specified section
	*/
	bool  GetItemValueI(const string &strSectionName, const string &strItemName, int &nValue) const;

	/**
	* GetItemValueB: Get item value in Boolean format
	* @param strSectionName: The section name whose item vector would be checked 
	* @param strItemName: The item name specified to retrieve its corresponding item value
	* @return value: The value of the specified item within a specified section
	*/
	bool GetItemValueB(const string &strSectionName, const string &strItemName, bool &bValue) const;

	/**
	* GetItemValueF: Get item value in floating-point format
	* @param strSectionName: The section name whose item vector would be checked 
	* @param strItemName: The item name specified to retrieve its corresponding item value
	* @return value: The value of the specified item within a specified section
	*/
	bool GetItemValueF(const string &strSectionName, const string &strItemName, double &fValue) const;

	/**
	* SetItemValueS: Set item value using specified section name and item name in string format
	* @param strSectionName: The section name whose corresponding vector would be resized
	* @param strItemName: The item name whose corresponding slot would be used to save strValue
	* @param strValue: The value the user specify to set
	* @param bCreate: Whether to create section and item and its corresponding value if they are void
	* @return value: Whether setting item value succeeded
	*/
	bool SetItemValueS(const string &strSectionName, const string &strItemName, const string &strValue, bool bCreate = true);

	/**
	* SetItemValueI: Set item value using specified section name and item name in Integer format
	* @param strSectionName: The section name whose corresponding vector would be resized
	* @param strItemName: The item name whose corresponding slot would be used to save strValue
	* @param nValue: The value the user specify to set
	* @param bCreate: Whether to create section and item and its corresponding value if they are void
	* @return value: Whether setting item value succeeded
	*/
	bool SetItemValueI(const string &strSectionName, const string &strValueName, int nValue, bool bCreate = true);

	/**
	* SetItemValueB: Set item value using specified section name and item name in Boolean format
	* @param strSectionName: The section name whose corresponding vector would be resized
	* @param strItemName: The item name whose corresponding slot would be used to save strValue
	* @param bValue: The value the user specify to set
	* @param bCreate: Whether to create section and item and its corresponding value if they are void
	* @return value: Whether setting item value succeeded
	*/
	bool SetItemValueB(const string &strSectionName, const string &strValueName, bool bValue, bool bCreate = true);

	/**
	* SetItemValueF: Set item value using specified section name and item name in Floating-point format
	* @param strSectionName: The section name whose corresponding vector would be resized
	* @param strItemName: The item name whose corresponding slot would be used to save strValue
	* @param fValue: The value the user specify to set
	* @param bCreate: Whether to create section and item and its corresponding value if they are void
	* @return value: Whether setting item value succeeded
	*/
	bool SetItemValueF(const string &strSectionName, const string &strValueName, double fValue, bool bCreate = true);

	/**
	* DeleteSection: Delete section and its sub items.
	* @param strSectionName: The section name which would be deleted, including its sub items.
	* @return value: Whether deleting section succeeded
	*/
	bool DeleteSection(const string &strSectionName);

	/**
	* DeleteItem: Delete item according to its item name and its corresponding section name
	* @param strSectionName: The section name whose specified item would be deleted
	* @param strItemName: The item name which would be deleted, including its corresponding value
	* @return value: Whether deleting item succeeded
	*/
	bool DeleteItem(const string &strSectionName, const string &strItemName);

	/**
	* AddHeaderComment: Add line comment to the data structure in RAM
	* @param strComment: Read out comment line
	* @return void
	*/
	void AddHeaderComment(const string &strComment);

	/**
	* AddSectionComment: Overloading function, Adding ini file comments  to memory for manipulation
	* @param nSectionName: Section name specified to indicate which section this comment belongs to
	* @param strComment: Line comment inserted in to the data structure in RAM
	* @return value: Whether adding header comment succeeded
	*/
	bool AddSectionComment(const string &strSectionName, const string &strComment);

private:
	/**
	* StrCompare: Check string equal
	* @param str1: first string value
	* @param str2: second string value
	* @param bCaseSensitive: is case sensitive
	* @return value: Original string or upper case version of the original string
	*/
	bool StrCompare(const string& str1, const string& str2, bool bCaseSensitive = false) const;

	/**
	* GetSectionIndex: Get section index using specified section name
	* @param strSectionName: Section name whose index will be returned
	* @param nIndex [out] : the index value returned
	* @return true: succeed, false: section not found
	*/
	bool GetSectionIndex(const string &strSectionName, unsigned &nIndex) const;

	/**
	* GetSectionName: Get section name according to its index
	* @param nSectionIndex: Section index whose corresponding name would be retrieved
	* @return value: The section name of the corresponded section Index
	*/
	string GetSectionName(unsigned nSectionIndex) const;

	/**
	* GetItemCountInSection: Get item count in section specified by its index
	* @param nSectionIndex: Item count in the specified section would be retrieved
	* @return value: The item count in the specified section
	*/
	unsigned GetItemCountInSection(unsigned nSectionIndex);

	/**
	* GetItemName: Get item name according to the specified section Index and item Index
	* @param nSectionIndex: The section index whose item vector would be checked
	* @param nItemIndex: The item index specified to retrieve its corresponding item name
	* @return value: The name of the specified item within a specified section
	*/
	string GetItemName(unsigned nSectionIndex, unsigned nItemIndex) const;

	/**
	* GetItemIndex: Get item corresponded index specified using its name
	* @param nSectionIndex: The section index whose item vector would be checked with strItemName
	* @param strItemName: The item name specified to locate its index within a specific section
	* @param nIndex [out]: the index value returned
	* @return true: succeed, false: section not found
	*/
	bool GetItemIndex(unsigned nSectionNdex, const string &strItemName, unsigned &nIndex) const;

	/**
	* GetItemValue: Get item value according to the specified section Index and item Index
	* @param nSectionIndex: The section index whose item vector would be checked
	* @param nItemIndex: The item index specified to retrieve its corresponding item value
	* @param isSucceeded: Whether getting item value operation succeeded
	* @return value: The value of the specified item within a specified section
	*/
	bool GetItemValue(unsigned nSectionIndex, unsigned nItemIndex, string &strValue);

	/**
	* SetItemValue: Set item value using specified section Index and item Index
	* @param nSectionIndex: The section index whose corresponding vector would be resized
	* @param nItemIndex: The item index whose corresponding slot would be used to save strValue
	* @param strValue: The value the user specify to set
	* @return value: Whether setting item value succeeded
	*/
	bool SetItemValue(unsigned nSectionIndex, unsigned nItemIndex, const string &strValue);

	/**
	* AddSectionComment: Adding ini file comments  to memory for manipulation
	* @param nSectionIndex: Section Index specified to indicate which section this comment belongs to
	* @param strComment: Line comment inserted in to the data structure in RAM
	* @return value: Whether adding header comment succeeded
	*/
	bool AddSectionComment(unsigned  nSectionIndex, const string &strComment);

private:
	string m_strFilePath;
	vector<IniSection>  m_vecBlocks; 
	vector<string> m_vecSections; 
	vector<string> m_vecComments;
};

#endif