#pragma once

/**
 * Heroes SDK
 * INI file parser.
 */

#ifndef INIFILE_H
#define INIFILE_H

#include <cstdio>
#include <string>
#include <unordered_map>

class IniFile;

/**
 * Individual INI group.
 */
class IniGroup
{
public:
	bool hasKey(const std::string& key) const;
	bool hasKeyNonEmpty(const std::string& key) const;

	const std::unordered_map<std::string, std::string>* data() const;

	std::string getString(const std::string& key, const std::string& def = std::string()) const;
	std::wstring getWString(const std::string& key, const std::wstring& def = std::wstring()) const;
	bool getBool(const std::string& key, bool def = false) const;
	int getIntRadix(const std::string& key, int radix, int def = 0) const;
	int getInt(const std::string& key, int def = 0) const;
	float getFloat(const std::string& key, float def = 0) const;

	void setString(const std::string& key, const std::string& val);
	void setWString(const std::string& key, const std::wstring& val);
	void setBool(const std::string& key, bool val);
	void setIntRadix(const std::string& key, int radix, int val);
	void setInt(const std::string& key, int val);
	void setFloat(const std::string& key, float val);

	bool removeKey(const std::string& key);

	using iterator = std::unordered_map<std::string, std::string>::iterator;
	using const_iterator = std::unordered_map<std::string, std::string>::const_iterator;

	iterator begin();
	const_iterator cbegin() const;
	iterator end();
	const_iterator cend() const;

protected:
	friend class IniFile;

	/**
	 * INI section data.
	 * - Key: Key name. (UTF-8)
	 * - Value: Value. (UTF-8)
	 */
	std::unordered_map<std::string, std::string> m_data;
};

inline auto begin(IniGroup& x)
{
	return x.begin();
}

inline auto end(IniGroup& x)
{
	return x.end();
}

inline auto cbegin(const IniGroup& x)
{
	return x.cbegin();
}

inline auto cend(const IniGroup& x)
{
	return x.cend();
}

/**
 * INI file.
 * Contains multiple INI groups.
 */
class IniFile
{
public:
	explicit IniFile(const std::string& filename);
	explicit IniFile(const std::wstring& filename);
	explicit IniFile(const char* filename);
	explicit IniFile(const wchar_t* filename);
	explicit IniFile(FILE* f);
	~IniFile();

	IniGroup* getGroup(const std::string& section);
	const IniGroup* getGroup(const std::string& section) const;
	IniGroup* createGroup(const std::string& section);

	bool hasGroup(const std::string& section) const;
	bool hasKey(const std::string& section, const std::string& key) const;
	bool hasKeyNonEmpty(const std::string& section, const std::string& key) const;

	std::string getString(const std::string& section, const std::string& key, const std::string& def = std::string()) const;
	std::wstring getWString(const std::string& section, const std::string& key, const std::wstring& def = std::wstring()) const;
	bool getBool(const std::string& section, const std::string& key, bool def = false) const;
	int getIntRadix(const std::string& section, const std::string& key, int radix, int def = 0) const;
	int getInt(const std::string& section, const std::string& key, int def = 0) const;
	float getFloat(const std::string& section, const std::string& key, float def = 0) const;

	void setString(const std::string& section, const std::string& key, const std::string& val);
	void setWString(const std::string& section, const std::string& key, const std::wstring& val);
	void setBool(const std::string& section, const std::string& key, bool val);
	void setIntRadix(const std::string& section, const std::string& key, int radix, int val);
	void setInt(const std::string& section, const std::string& key, int val);
	void setFloat(const std::string& section, const std::string& key, float val);

	bool removeGroup(const std::string& group);
	bool removeKey(const std::string& section, const std::string& key);

	void save(const std::string& filename) const;
	void save(const std::wstring& filename) const;
	void save(FILE* f) const;

	using iterator = std::unordered_map<std::string, IniGroup*>::iterator;
	using const_iterator = std::unordered_map<std::string, IniGroup*>::const_iterator;

	iterator begin();
	const_iterator cbegin() const;
	iterator end();
	const_iterator cend() const;

protected:
	void load(FILE* f);
	void clear();
	static std::string escape(const std::string& str, bool sec, bool key);

	/**
	 * INI groups.
	 * - Key: Section name. (UTF-8)
	 * - Value: IniGroup.
	 */
	std::unordered_map<std::string, IniGroup*> m_groups;
};

inline auto begin(IniFile& x)
{
	return x.begin();
}

inline auto end(IniFile& x)
{
	return x.end();
}

inline auto cbegin(const IniFile& x)
{
	return x.cbegin();
}

inline auto cend(const IniFile& x)
{
	return x.cend();
}

#endif /* INIFILE_H */
