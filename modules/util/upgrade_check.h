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

#ifndef MODULES_UTIL_UPGRADE_CHECK_H_
#define MODULES_UTIL_UPGRADE_CHECK_H_

#include "modules/util/upgrade_checker/upgrade_check_config.h"

namespace mysqlshdk {
namespace db {
class ISession;
class IRow;
}  // namespace db
}  // namespace mysqlshdk

namespace mysqlsh {
namespace upgrade_checker {

/**
 * Checks if server is ready for upgrade.
 *
 * @param config upgrade configuration
 *
 * @returns true if server is eligible for upgrade (there were no errors)
 */
bool check_for_upgrade(const Upgrade_check_config &config);

}  // namespace upgrade_checker
}  // namespace mysqlsh

#endif  // MODULES_UTIL_UPGRADE_CHECK_H_
