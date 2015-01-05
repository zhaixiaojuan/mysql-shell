/*
 * Copyright (c) 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include "shellcore/types.h"

namespace shcore
{

class Object_factory
{
public:
  virtual ~Object_factory() {}

  virtual std::string name() const = 0;

  //! Factory method... creates an instance of the class
  virtual boost::shared_ptr<Object_bridge> construct(const Argument_list &args) = 0;

  //! Factory method... recreates an instance of the class through the repr value of it
//  virtual boost::shared_ptr<Object_bridge> construct_from_repr(const std::string &repr) = 0;

public:
  //! Registers a metaclass
  static void register_factory(const std::string &package, Object_factory *fac);

  static boost::shared_ptr<Object_bridge> call_constructor(const std::string &package, const std::string &name,
                                                           const Argument_list &args);

  static std::vector<std::string> package_names();
  static std::vector<std::string> package_contents(const std::string &package);

  static bool has_package(const std::string &package);

  // call_constructor_with_repr(const std::string &repr)
};

};
