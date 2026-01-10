#include "iniParser.h"
#include <algorithm>
#include <sstream>


static std::string& trim(std::string& s) {
    if (s.empty()) return s;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) {
        return !std::isspace(c);
        }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char c) {
        return !std::isspace(c);
        }).base(), s.end());
    return s;
}

// Helper: convert to uppercase
static std::string toUpper(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::toupper);
    return r;
}

CIniParser::CIniParser() {}

CIniParser::~CIniParser() {}

bool CIniParser::ReadFile(const std::string& strPath) {
    EraseAllContent();
    m_strFilePath = strPath;

    std::ifstream file(strPath.c_str());
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::string currentSection;
    unsigned sectionIndex = 0;

    while (std::getline(file, line)) {
        // Remove comments (; or #)
        size_t commentPos = line.find(';');
        if (commentPos == std::string::npos) commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        trim(line);
        if (line.empty()) continue;

        // Check for [Section]
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
            trim(currentSection);
            if (!currentSection.empty()) {
                m_vecSections.push_back(currentSection);
                m_vecBlocks.emplace_back();
                sectionIndex = static_cast<unsigned>(m_vecSections.size() - 1);
            }
            continue;
        }

        // Parse key = value
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;

        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);
        trim(key);
        trim(value);

        if (!key.empty() && !currentSection.empty()) {
            m_vecBlocks[sectionIndex].vecItems.push_back(key);
            m_vecBlocks[sectionIndex].vecValues.push_back(value);
            m_vecBlocks[sectionIndex].vecComments.push_back(""); // No inline comment support in this impl
        }
    }

    return true;
}

bool CIniParser::WriteFile(const std::string& strPath) {
    std::ofstream file(strPath.c_str());
    if (!file.is_open()) {
        return false;
    }

    // Write header comments
    for (const auto& comment : m_vecComments) {
        file << "; " << comment << "\n";
    }

    for (size_t i = 0; i < m_vecSections.size(); ++i) {
        file << "[" << m_vecSections[i] << "]\n";
        const auto& block = m_vecBlocks[i];
        for (size_t j = 0; j < block.vecItems.size(); ++j) {
            file << block.vecItems[j] << "=" << block.vecValues[j] << "\n";
        }
        file << "\n";
    }

    return true;
}

void CIniParser::EraseAllContent() {
    m_vecSections.clear();
    m_vecBlocks.clear();
    m_vecComments.clear();
}

unsigned CIniParser::GetSectionCount() const {
    return static_cast<unsigned>(m_vecSections.size());
}

unsigned CIniParser::AddSection(const std::string& strSectionName) {
    m_vecSections.push_back(strSectionName);
    m_vecBlocks.emplace_back();
    return static_cast<unsigned>(m_vecSections.size() - 1);
}

unsigned CIniParser::GetItemCountInSection(const std::string& strSectionName) {
    unsigned index;
    if (!GetSectionIndex(strSectionName, index)) {
        return 0;
    }
    return static_cast<unsigned>(m_vecBlocks[index].vecItems.size());
}

std::string CIniParser::GetItemName(const std::string& strSectionName, unsigned nItemIndex) const {
    unsigned index;
    if (!GetSectionIndex(strSectionName, index)) {
        return "";
    }
    const auto& items = m_vecBlocks[index].vecItems;
    if (nItemIndex >= items.size()) {
        return "";
    }
    return items[nItemIndex];
}

bool CIniParser::GetItemValueS(const std::string& strSectionName, const std::string& strItemName, std::string& strValue) const {
    unsigned secIndex;
    if (!GetSectionIndex(strSectionName, secIndex)) {
        return false;
    }
    const auto& block = m_vecBlocks[secIndex];
    for (size_t i = 0; i < block.vecItems.size(); ++i) {
        if (StrCompare(block.vecItems[i], strItemName)) {
            strValue = block.vecValues[i];
            return true;
        }
    }
    return false;
}

bool CIniParser::GetItemValueI(const std::string& strSectionName, const std::string& strItemName, int& nValue) const {
    std::string strVal;
    if (!GetItemValueS(strSectionName, strItemName, strVal)) {
        return false;
    }
    try {
        nValue = std::stoi(strVal);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool CIniParser::GetItemValueB(const std::string& strSectionName, const std::string& strItemName, bool& bValue) const {
    std::string strVal;
    if (!GetItemValueS(strSectionName, strItemName, strVal)) {
        return false;
    }
    std::string upperVal = toUpper(strVal);
    if (upperVal == "1" || upperVal == "TRUE" || upperVal == "YES" || upperVal == "ON") {
        bValue = true;
        return true;
    }
    else if (upperVal == "0" || upperVal == "FALSE" || upperVal == "NO" || upperVal == "OFF") {
        bValue = false;
        return true;
    }
    return false;
}

bool CIniParser::GetItemValueF(const std::string& strSectionName, const std::string& strItemName, double& fValue) const {
    std::string strVal;
    if (!GetItemValueS(strSectionName, strItemName, strVal)) {
        return false;
    }
    try {
        fValue = std::stod(strVal);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool CIniParser::SetItemValueS(const std::string& strSectionName, const std::string& strItemName, const std::string& strValue, bool bCreate) {
    unsigned secIndex;
    if (!GetSectionIndex(strSectionName, secIndex)) {
        if (!bCreate) return false;
        secIndex = AddSection(strSectionName);
    }

    auto& block = m_vecBlocks[secIndex];
    for (size_t i = 0; i < block.vecItems.size(); ++i) {
        if (StrCompare(block.vecItems[i], strItemName)) {
            block.vecValues[i] = strValue;
            return true;
        }
    }

    // Not found, add new
    if (bCreate) {
        block.vecItems.push_back(strItemName);
        block.vecValues.push_back(strValue);
        block.vecComments.push_back("");
        return true;
    }
    return false;
}

bool CIniParser::SetItemValueI(const std::string& strSectionName, const std::string& strValueName, int nValue, bool bCreate) {
    std::ostringstream oss;
    oss << nValue;
    return SetItemValueS(strSectionName, strValueName, oss.str(), bCreate);
}

bool CIniParser::SetItemValueB(const std::string& strSectionName, const std::string& strValueName, bool bValue, bool bCreate) {
    return SetItemValueS(strSectionName, strValueName, bValue ? "1" : "0", bCreate);
}

bool CIniParser::SetItemValueF(const std::string& strSectionName, const std::string& strValueName, double fValue, bool bCreate) {
    std::ostringstream oss;
    oss.precision(6);
    oss << std::fixed << fValue;
    std::string strVal = oss.str();
    // Remove trailing zeros
    strVal.erase(strVal.find_last_not_of('0') + 1, std::string::npos);
    if (strVal.back() == '.') strVal += '0';
    return SetItemValueS(strSectionName, strValueName, strVal, bCreate);
}

bool CIniParser::DeleteSection(const std::string& strSectionName) {
    unsigned index;
    if (!GetSectionIndex(strSectionName, index)) {
        return false;
    }
    m_vecSections.erase(m_vecSections.begin() + index);
    m_vecBlocks.erase(m_vecBlocks.begin() + index);
    return true;
}

bool CIniParser::DeleteItem(const std::string& strSectionName, const std::string& strItemName) {
    unsigned secIndex;
    if (!GetSectionIndex(strSectionName, secIndex)) {
        return false;
    }
    auto& block = m_vecBlocks[secIndex];
    for (auto it = block.vecItems.begin(); it != block.vecItems.end(); ++it) {
        if (StrCompare(*it, strItemName)) {
            size_t idx = std::distance(block.vecItems.begin(), it);
            block.vecItems.erase(it);
            block.vecValues.erase(block.vecValues.begin() + idx);
            block.vecComments.erase(block.vecComments.begin() + idx);
            return true;
        }
    }
    return false;
}

void CIniParser::AddHeaderComment(const std::string& strComment) {
    m_vecComments.push_back(strComment);
}

bool CIniParser::AddSectionComment(const std::string& strSectionName, const std::string& strComment) {
    unsigned index;
    if (!GetSectionIndex(strSectionName, index)) {
        return false;
    }
    // This implementation does not store per-item comments, so we ignore
    return true;
}

// --- Private Methods ---

bool CIniParser::StrCompare(const std::string& str1, const std::string& str2, bool bCaseSensitive) const {
    if (bCaseSensitive) {
        return str1 == str2;
    }
    return toUpper(str1) == toUpper(str2);
}

bool CIniParser::GetSectionIndex(const std::string& strSectionName, unsigned& nIndex) const {
    for (unsigned i = 0; i < m_vecSections.size(); ++i) {
        if (StrCompare(m_vecSections[i], strSectionName)) {
            nIndex = i;
            return true;
        }
    }
    return false;
}

std::string CIniParser::GetSectionName(unsigned nSectionIndex) const {
    if (nSectionIndex >= m_vecSections.size()) {
        return "";
    }
    return m_vecSections[nSectionIndex];
}

unsigned CIniParser::GetItemCountInSection(unsigned nSectionIndex) {
    if (nSectionIndex >= m_vecBlocks.size()) {
        return 0;
    }
    return static_cast<unsigned>(m_vecBlocks[nSectionIndex].vecItems.size());
}

std::string CIniParser::GetItemName(unsigned nSectionIndex, unsigned nItemIndex) const {
    if (nSectionIndex >= m_vecBlocks.size()) return "";
    const auto& items = m_vecBlocks[nSectionIndex].vecItems;
    if (nItemIndex >= items.size()) return "";
    return items[nItemIndex];
}

bool CIniParser::GetItemIndex(unsigned nSectionIndex, const std::string& strItemName, unsigned& nIndex) const {
    if (nSectionIndex >= m_vecBlocks.size()) return false;
    const auto& items = m_vecBlocks[nSectionIndex].vecItems;
    for (unsigned i = 0; i < items.size(); ++i) {
        if (StrCompare(items[i], strItemName)) {
            nIndex = i;
            return true;
        }
    }
    return false;
}

bool CIniParser::GetItemValue(unsigned nSectionIndex, unsigned nItemIndex, std::string& strValue) {
    if (nSectionIndex >= m_vecBlocks.size()) return false;
    auto& values = m_vecBlocks[nSectionIndex].vecValues;
    if (nItemIndex >= values.size()) return false;
    strValue = values[nItemIndex];
    return true;
}

bool CIniParser::SetItemValue(unsigned nSectionIndex, unsigned nItemIndex, const std::string& strValue) {
    if (nSectionIndex >= m_vecBlocks.size()) return false;
    auto& values = m_vecBlocks[nSectionIndex].vecValues;
    if (nItemIndex >= values.size()) return false;
    values[nItemIndex] = strValue;
    return true;
}

bool CIniParser::AddSectionComment(unsigned nSectionIndex, const std::string& strComment) {
    // Not implemented in this version
    return true;
}