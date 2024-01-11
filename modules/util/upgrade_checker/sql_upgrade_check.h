/*
 * Copyright (c) 2017, 2023, Oracle and/or its affiliates.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2.0,
 * as published by the Free Software Foundation.
 *
 * This program is also distributed with certain software (including
 * but not limited to OpenSSL) that is licensed under separate terms, as
 * designated in a particular file or component or in included license
 * documentation.  The authors of MySQL hereby grant you an additional
 * permission to link the program and your derivative works with the
 * separately licensed software that they have included with MySQL.
 * This program is distributed in the hope that it will be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License, version 2.0, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef MODULES_UTIL_UPGRADE_CHECKER_SQL_UPGRADE_CHECK_H_
#define MODULES_UTIL_UPGRADE_CHECKER_SQL_UPGRADE_CHECK_H_

#include <forward_list>
#include <memory>
#include <string>
#include <vector>

#include "modules/util/upgrade_checker/common.h"
#include "modules/util/upgrade_checker/upgrade_check.h"
#include "mysqlshdk/libs/db/row.h"

namespace mysqlsh {
namespace upgrade_checker {

class Sql_upgrade_check : public Upgrade_check {
 public:
  Sql_upgrade_check(const char *name, const char *title,
                    std::vector<std::string> &&queries,
                    Upgrade_issue::Level level = Upgrade_issue::WARNING,
                    const char *advice = "",
                    const char *minimal_version = nullptr,
                    std::forward_list<std::string> &&set_up =
                        std::forward_list<std::string>(),
                    std::forward_list<std::string> &&clean_up =
                        std::forward_list<std::string>());

  std::vector<Upgrade_issue> run(
      const std::shared_ptr<mysqlshdk::db::ISession> &session,
      const Upgrade_info &server_info) override;

  const std::vector<std::string> &get_queries() const { return m_queries; }

 protected:
  virtual Upgrade_issue parse_row(const mysqlshdk::db::IRow *row);
  virtual void add_issue(const mysqlshdk::db::IRow *row,
                         std::vector<Upgrade_issue> *issues);
  const char *get_description_internal() const override;
  const char *get_title_internal() const override;
  Upgrade_issue::Level get_level() const override { return m_level; }

  std::vector<std::string> m_queries;
  std::forward_list<std::string> m_set_up;
  std::forward_list<std::string> m_clean_up;
  const Upgrade_issue::Level m_level;
  std::string m_title;
  std::string m_advice;
  std::string m_doc_link;
  const char *m_minimal_version;
};

}  // namespace upgrade_checker
}  // namespace mysqlsh

#endif  // MODULES_UTIL_UPGRADE_CHECKER_SQL_UPGRADE_CHECK_H_