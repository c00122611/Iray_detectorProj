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
    // 步骤1：初始化，清空原有数据，避免残留影响
    EraseAllContent();
    m_strFilePath = strPath;

    // 步骤2：打开文件，校验文件可用性
    std::ifstream file(strPath.c_str());
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::string currentSection;  // 持久化当前节上下文
    unsigned currentSectionIndex = 0; // 持久化当前节在容器中的索引

    while (std::getline(file, line)) {
        // 步骤3：处理注释（; 或 #），截断注释部分，保留注释前的有效内容
        size_t commentPos = line.find(';');
        if (commentPos == std::string::npos) {
            commentPos = line.find('#');
        }
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        // 步骤4：修剪前后空格，空行直接跳过（核心：不重置当前节上下文）
        trim(line);
        if (line.empty()) {
            continue;
        }

        // 步骤5：解析节 [Section]，确保节必然被存入容器（核心修复点）
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
            trim(currentSection);

            // 仅当节名非空时，处理容器存入
            if (!currentSection.empty()) {
                unsigned existingIndex;
                // 先判断节是否已存在，避免重复添加
                if (GetSectionIndex(currentSection, existingIndex)) {
                    // 节已存在，切换到已有索引，保持上下文连续
                    currentSectionIndex = existingIndex;
                }
                else {
                    // 节不存在，强制添加到两个容器，建立对应关系
                    m_vecSections.push_back(currentSection);
                    m_vecBlocks.emplace_back(); // 构造空Block，与节一一对应
                    currentSectionIndex = static_cast<unsigned>(m_vecSections.size() - 1);

                    // 额外校验：确保索引与容器大小匹配（防止容器操作异常）
                    if (currentSectionIndex >= m_vecBlocks.size()) {
                        m_vecBlocks.emplace_back(); // 兜底，避免索引越界
                    }
                }
            }
            continue;
        }

        // 步骤6：解析 key=value 键值对，确保存入对应节的容器
        size_t eqPos = line.find('=');
        // 排除无效键值对（无=、=在开头、=在结尾）
        if (eqPos == std::string::npos || eqPos == 0 || eqPos == line.size() - 1) {
            continue;
        }

        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);
        trim(key);
        trim(value);

        // 步骤7：严格校验存入条件，确保字段存入正确的节容器
        if (!key.empty() && !currentSection.empty()) {
            // 双重校验：索引合法 + 节名匹配（防止上下文错乱）
            if (currentSectionIndex < m_vecBlocks.size() &&
                StrCompare(m_vecSections[currentSectionIndex], currentSection)) {
                IniSection& currentBlock = m_vecBlocks[currentSectionIndex];
                // 避免重复添加相同key（可选，根据业务需求决定是否保留）
                bool keyDuplicate = false;
                for (const auto& existingKey : currentBlock.vecItems) {
                    if (StrCompare(existingKey, key)) {
                        keyDuplicate = true;
                        break;
                    }
                }
                if (!keyDuplicate) {
                    currentBlock.vecItems.push_back(key);
                    currentBlock.vecValues.push_back(value);
                    currentBlock.vecComments.push_back("");
                }
            }
        }
    }

    // 步骤8：清理无有效字段的空节，避免容器中留存无效数据
    CleanEmptySections();

    return true;
}

// 实现空节清理私有方法
void CIniParser::CleanEmptySections() {
    // 倒序遍历，避免删除元素后索引错乱
    for (int i = static_cast<int>(m_vecSections.size()) - 1; i >= 0; --i) {
        // 仅当 Block 中无有效字段时，删除对应节
        if (m_vecBlocks[i].vecItems.empty()) {
            m_vecSections.erase(m_vecSections.begin() + i);
            m_vecBlocks.erase(m_vecBlocks.begin() + i);
        }
    }
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

bool CIniParser::IsSectionExists(const std::string& strSectionName) const {
    unsigned index;
    return GetSectionIndex(strSectionName, index);
}