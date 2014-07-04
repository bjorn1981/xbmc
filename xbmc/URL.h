#pragma once
/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "utils/StdString.h"
#include "utils/UrlOptions.h"

#ifdef TARGET_WINDOWS
#undef SetPort // WIN32INCLUDES this is defined as SetPortA in WinSpool.h which is being included _somewhere_
#endif

class CURL
{
public:
  explicit CURL(const CStdString& strURL);
  CURL();
  virtual ~CURL(void);

  // explicit equals operator for std::string comparison
  bool operator==(const std::string &url) const { return Get() == url; }

  void Reset();
  void Parse(const CStdString& strURL);
  void SetFileName(const CStdString& strFileName);
  void SetHostName(const CStdString& strHostName);
  void SetUserName(const CStdString& strUserName);
  void SetPassword(const CStdString& strPassword);
  void SetProtocol(const CStdString& strProtocol);
  void SetOptions(const CStdString& strOptions);
  void SetProtocolOptions(const CStdString& strOptions);
  void SetPort(int port);

  bool HasPort() const;

  int GetPort() const;
  const CStdString& GetHostName() const;
  const CStdString& GetDomain() const;
  const CStdString& GetUserName() const;
  const CStdString& GetPassWord() const;
  const CStdString& GetFileName() const;
  const CStdString& GetProtocol() const;
  const CStdString GetTranslatedProtocol() const;
  const CStdString& GetFileType() const;
  const CStdString& GetShareName() const;
  const CStdString& GetOptions() const;
  const CStdString& GetProtocolOptions() const;
  const CStdString GetFileNameWithoutPath() const; /* return the filename excluding path */

  char GetDirectorySeparator() const;

  CStdString Get() const;
  std::string GetWithoutUserDetails(bool redact = false) const;
  CStdString GetWithoutFilename() const;
  std::string GetRedacted() const;
  static std::string GetRedacted(const std::string& path);
  bool IsLocal() const;
  bool IsLocalHost() const;
  static bool IsFileOnly(const CStdString &url); ///< return true if there are no directories in the url.
  static bool IsFullPath(const CStdString &url); ///< return true if the url includes the full path
  static std::string Decode(const std::string& strURLData);
  static std::string Encode(const std::string& strURLData);

  /*! \brief Check whether a URL is a given URL scheme.
   Comparison is case-insensitive as per RFC1738
   \param type a lower-case scheme name, e.g. "smb".
   \return true if the url is of the given scheme, false otherwise.
   */
  bool IsProtocol(const char *type) const
  {
    return IsProtocolEqual(m_strProtocol, type);
  }

  /*! \brief Check whether a URL protocol is a given URL scheme.
   Both parameters MUST be lower-case.  Typically this would be called using
   the result of TranslateProtocol() which enforces this for protocol.
   \param protocol a lower-case scheme name, e.g. "ftp"
   \param type a lower-case scheme name, e.g. "smb".
   \return true if the url is of the given scheme, false otherwise.
   */
  static bool IsProtocolEqual(const std::string& protocol, const char *type);

  /*! \brief Check whether a URL is a given filetype.
   Comparison is effectively case-insensitive as both the parameter
   and m_strFileType are lower-case.
   \param type a lower-case filetype, e.g. "mp3".
   \return true if the url is of the given filetype, false otherwise.
   */
  bool IsFileType(const char *type) const
  {
    return m_strFileType == type;
  }

  void GetOptions(std::map<CStdString, CStdString> &options) const;
  bool HasOption(const CStdString &key) const;
  bool GetOption(const CStdString &key, CStdString &value) const;
  CStdString GetOption(const CStdString &key) const;
  void SetOption(const CStdString &key, const CStdString &value);
  void RemoveOption(const CStdString &key);

  void GetProtocolOptions(std::map<CStdString, CStdString> &options) const;
  bool HasProtocolOption(const CStdString &key) const;
  bool GetProtocolOption(const CStdString &key, CStdString &value) const;
  CStdString GetProtocolOption(const CStdString &key) const;
  void SetProtocolOption(const CStdString &key, const CStdString &value);
  void RemoveProtocolOption(const CStdString &key);

protected:
  int m_iPort;
  CStdString m_strHostName;
  CStdString m_strShareName;
  CStdString m_strDomain;
  CStdString m_strUserName;
  CStdString m_strPassword;
  CStdString m_strFileName;
  CStdString m_strProtocol;
  CStdString m_strFileType;
  CStdString m_strOptions;
  CStdString m_strProtocolOptions;
  CUrlOptions m_options;
  CUrlOptions m_protocolOptions;
};
