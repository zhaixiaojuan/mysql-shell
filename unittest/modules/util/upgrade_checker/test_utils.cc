/*
 * Copyright (c) 2024, Oracle and/or its affiliates.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2.0,
 * as published by the Free Software Foundation.
 *
 * This program is designed to work with certain software (including
 * but not limited to OpenSSL) that is licensed under separate terms,
 * as designated in a particular file or component or in included license
 * documentation.  The authors of MySQL hereby grant you an additional
 * permission to link the program and your derivative works with the
 * separately licensed software that they have either included with
 * the program or referenced in the documentation.
 *
 * This program is distributed in the hope that it will be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License, version 2.0, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "unittest/modules/util/upgrade_checker/test_utils.h"

#include <iostream>

#include "mysqlshdk/include/scripting/types.h"

namespace mysqlsh {
namespace upgrade_checker {
Upgrade_info upgrade_info(Version server, Version target,
                          std::string server_os) {
  Upgrade_info ui;

  ui.server_version = std::move(server);
  ui.target_version = std::move(target);
  ui.server_os = std::move(server_os);

  return ui;
}

Upgrade_info upgrade_info(const std::string &server,
                          const std::string &target) {
  return upgrade_info(Version(server), Version(target));
}

Version before_version(const Version &version) {
  auto major = version.get_major();
  auto minor = version.get_minor();
  auto patch = version.get_patch();

  if (--patch < 0) {
    patch = 99;
    if (--minor < 0) {
      minor = 99;
      if (--major < 0) {
        throw std::logic_error("Invalid version used!");
      }
    }
  }

  return Version(major, minor, patch);
}

Version before_version(const Version &version, int count) {
  Version result = version;
  while (count--) {
    result = before_version(result);
  }

  return result;
}

Version after_version(const Version &version) {
  auto major = version.get_major();
  auto minor = version.get_minor();
  auto patch = version.get_patch();

  if (++patch > 99) {
    patch = 0;
    if (++minor > 99) {
      minor = 0;
      if (major > 99) {
        throw std::logic_error("Invalid version used!");
      }
    }
  }

  return Version(major, minor, patch);
}

Version after_version(const Version &version, int count) {
  Version result = version;
  while (count--) {
    result = after_version(result);
  }

  return result;
}

std::string Upgrade_checker_test::deploy_sandbox(
    const shcore::Dictionary_t &conf, int *sb_port) {
  assert(sb_port);

  for (auto port : _mysql_sandbox_ports) {
    try {
      testutil->deploy_raw_sandbox(port, "root", conf);
      *sb_port = port;
      return shcore::str_format("root:root@localhost:%d", port);
    } catch (const std::runtime_error &err) {
      std::cerr << err.what();
      testutil->destroy_sandbox(port, true);
    }
  }

  return "";
}

}  // namespace upgrade_checker
}  // namespace mysqlsh
