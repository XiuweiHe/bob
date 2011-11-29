/**
 * @file python/io/src/version.cc
 * @author Andre Anjos <andre.anjos@idiap.ch>
 * @date Tue 29 Nov 2011 10:53:16 CET
 *
 * @brief Describes ways to retrieve version information about all dependent
 * packages.
 *
 * Copyright (C) 2011 Idiap Reasearch Institute, Martigny, Switzerland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/python.hpp>
#include <boost/format.hpp>
#include <cstdlib>

#if defined(HAVE_IMAGE_MAGICK_PP)
#include <ImageMagick/Magick++.h>
#endif

extern "C" {
#include <hdf5.h>

#if defined(HAVE_FFMPEG)
#  include <libavformat/avformat.h>
#  include <libavcodec/avcodec.h>
#  include <libavutil/avutil.h>
#  include <libswscale/swscale.h>
#endif 

#ifdef HAVE_MATIO
#include <matio.h>
#endif
}

using namespace boost::python;

/**
 * Version of HDF5 support
 */
static str hdf5_version() {
  boost::format f("%s.%s.%s");
  f % BOOST_PP_STRINGIZE(H5_VERS_MAJOR);
  f % BOOST_PP_STRINGIZE(H5_VERS_MINOR);
  f % BOOST_PP_STRINGIZE(H5_VERS_RELEASE);
  return str(f.str());
}

/**
 * FFmpeg version
 */
static dict ffmpeg_version() {
  dict v;
#if defined(HAVE_FFMPEG)
# if defined(FFMPEG_VERSION)
  if (std::strlen(FFMPEG_VERSION)) v["ffmpeg"] = str(FFMPEG_VERSION);
# endif
  v["avformat"] = str(BOOST_PP_STRINGIZE(LIBAVFORMAT_VERSION));
  v["avcodec"] = str(BOOST_PP_STRINGIZE(LIBAVFORMAT_VERSION));
  v["avutil"] = str(BOOST_PP_STRINGIZE(LIBAVUTIL_VERSION));
  v["swscale"] = str(BOOST_PP_STRINGIZE(LIBSWSCALE_VERSION));
#else
  v["ffmpeg"] = str("unavailable");
#endif
  return v;
}

/**
 * ImageMagick version
 */
static str magick_version() {
#if defined(HAVE_IMAGE_MAGICK_PP)
  return str(MagickLibVersionText);
#else
  return str("unavailable");
#endif
}

/**
 * Matio, if compiled with such support
 */
static str matio_version() {
#ifdef HAVE_MATIO
  boost::format f("%s.%s.%s");
  f % BOOST_PP_STRINGIZE(MATIO_MAJOR_VERSION);
  f % BOOST_PP_STRINGIZE(MATIO_MINOR_VERSION);
  f % BOOST_PP_STRINGIZE(MATIO_RELEASE_LEVEL);
  return str(f.str());
#else
  return str("unavailable");
#endif
}

void bind_io_version() {
  dict vdict;
  vdict["HDF5"] = hdf5_version();
  vdict["FFmpeg"] = ffmpeg_version();
  vdict["Image Magick"] = magick_version();
  vdict["MatIO"] = matio_version();
  scope().attr("version") = vdict;
}
