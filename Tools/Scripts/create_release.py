#
# The MIT License
# Copyright (c) 2007 Kimmo Varis
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# $Id$

# This script prepares a WinMerge release
# Tasks it does:
# - cleans previous build files from folders
# - builds WinMerge.exe and WinMergeU.exe
# - builds pcre, expat and scew (through project file)

# Tools needed:
# - Python 2.5 :)
# - Subversion command line binaries

# Subversion binary
svn_binary = 'C:\\Program Files\\Subversion\\bin\\svn.exe'
# Path to VS binary to run
vs_path = 'C:\\Program Files\\Microsoft Visual Studio .NET 2003\\Common7\\IDE'
# VS command
vs_bin = 'devenv.com'
# Relative path where to create a release folder
dist_root_folder = '/WinMerge/Releases'


from subprocess import *
import os
import os.path
import sys
import getopt
import shutil

def cleanup_build():
    """Deletes all build files around folder structure"""

    print 'Delete old build files...'
    winmerge_temp = 'BuildTmp'
    if os.path.exists(winmerge_temp):
        print 'Remove folder %s' % winmerge_temp
        shutil.rmtree(winmerge_temp, True)
    else:
        print 'Skipping folder %s' % winmerge_temp
        

# Commented out as it seems running prelink and postbuild bat files
# don't work when build is started from this script.
# So you need to build expat, scew and pcre with VS first!

#    expat_temps = ['Externals/expat/lib/Release',
#                   'Externals/expat/lib/Release-w',
#                   'Externals/expat/lib/Release_static',
#                   'Externals/expat/lib/Release_static-w',
#                   'build/expat']
#    for path in expat_temps:
#        if os.path.exists(path):
#            print 'Remove folder %s' % path
#            shutil.rmtree(path, True) 
#        else:
#            print 'Skipping folder %s' % path
#
#    scew_temps = ['Externals/scew/win32/lib',
#                  'Externals/scew/win32/obj']
#    for path in scew_temps:
#        if os.path.exists(path):
#            print 'Remove folder: %s' % path
#            shutil.rmtree(path, True) 
#
#    pcre_temps = ['Externals/pcre/bin',
#                  'Externals/pcre/dll_pcre/o',
#                  'Externals/pcre/lib_pcre/o',
#                  'Externals/pcre/lib_pcreposix/o',
#                  'Externals/pcre/pcretest/o',
#                  'build/pcre']
#    for path in pcre_temps:
#        if os.path.exists(path):
#            print 'Remove folder %s' % path
#            shutil.rmtree(path, True) 

def set_resource_version(version):
    """Sets the version number to the resource."""

    call(['cscript', 'Src/SetResourceVersions.wsf', version])

def setup_translations():
    """Updates translation files by running scripts in Src/Languages."""

    # Scripts must be run from the directory where they reside
    curdir = os.getcwd()
    os.chdir('Src/Languages')
    print 'Updating POT file from Merge.rc file...'
    call(['cscript', '/nologo', 'CreateMasterPotFile.vbs'])
    print 'POT file updated'
    print 'Updating all PO files from POT file...'
    call(['cscript', '/nologo', 'UpdatePoFilesFromPotFile.vbs'])
    print 'All PO files updated.'
    os.chdir(curdir)

def get_and_create_dist_folder(folder):
    """Formats a folder name for version-specific distribution folder
    and creates the folder."""

    dist_folder = os.path.join(dist_root_folder, folder)
    os.mkdir(dist_folder)
    return dist_folder

def get_src_dist_folder(dist_folder, folder):
    """Format a source distribution folder path."""

    dist_src = os.path.join(dist_folder, folder + '-src')
    return dist_src

def svn_export(dist_src_folder):
    """Exports sources to distribution folder."""

    print 'Exporting sources to ' + dist_src_folder
    call([svn_binary, 'export', '--non-interactive', '.', dist_src_folder] )

def build_targets():
    """Builds all WinMerge targets."""

    vs_cmd = os.path.join(vs_path, vs_bin)
    #print vs_cmd

    build_winmerge(vs_cmd)
    build_shellext(vs_cmd)

def build_winmerge(vs_cmd):
    """Builds WinMerge executable targets."""

    cur_path = os.getcwd()
    solution_path = os.path.join(cur_path, 'Src\\Merge.vcproj')
    #print sol_path

    # devenv Src\Merge.dsp /rebuild Release
    print 'Compiling WinMerge executables'
    call([vs_cmd, solution_path, '/rebuild', 'Release'], shell=True)
    call([vs_cmd, solution_path, '/rebuild', 'UnicodeRelease'], shell=True)

def build_shellext(vs_cmd):
    """Builds 32-bit ShellExtension."""

    cur_path = os.getcwd()
    solution_path = os.path.join(cur_path, 'ShellExtension\\ShellExtension.vcproj')
    #print sol_path

    # devenv Src\Merge.dsp /rebuild Release
    call([vs_cmd, solution_path, '/rebuild', 'Release MinDependency'])
    call([vs_cmd, solution_path, '/rebuild', 'Unicode Release MinDependency'])

def build_manual():
    call(['Tools/Scripts/BuildManualChm.bat'])

def get_and_create_bin_folder(dist_folder, folder):
    """Formats and creates binary distribution folder."""

    bin_folder = os.path.join(dist_folder, folder + '-exe')
    os.mkdir(bin_folder)
    return bin_folder

def create_bin_folders(bin_folder, dist_src_folder):
    """Creates binary distribution folders."""

    cur_path = os.getcwd()
    os.chdir(bin_folder)
    lang_folder = os.path.join(bin_folder, 'Languages')
    os.mkdir(lang_folder)
    doc_folder = os.path.join(bin_folder, 'Docs')
    os.mkdir(doc_folder)
    filters_folder = os.path.join(bin_folder, 'Filters')
    plugins_folder = os.path.join(bin_folder, 'MergePlugins')
    os.chdir(cur_path)

    shutil.copy('build/mergerelease/WinMerge.exe', bin_folder)
    shutil.copy('build/mergeunicoderelease/WinMergeU.exe', bin_folder)

    shutil.copy('build/mergerelease/ShellExtension.dll', bin_folder)
    shutil.copy('build/mergeunicoderelease/ShellExtensionU.dll', bin_folder)
    shutil.copy('build/mergeunicoderelease/MergeLang.dll', bin_folder)
    shutil.copy('build/shellextensionx64/ShellExtensionX64.dll', bin_folder)
    shutil.copy('ShellExtension/Register.bat', bin_folder)
    shutil.copy('ShellExtension/UnRegister.bat', bin_folder)

    shutil.copy('build/pcre/pcre.dll', bin_folder)
    shutil.copy('build/expat/libexpat.dll', bin_folder)

    copy_po_files(lang_folder)
    filter_orig = os.path.join(dist_src_folder, 'Filters')
    shutil.copytree(filter_orig, filters_folder, False)

    # Copy compiled plugins dir and rename it
    plugin_dir = os.path.join(bin_folder, 'dlls')
    plugin_orig = os.path.join(dist_src_folder, 'Plugins/dlls')
    shutil.copytree(plugin_orig, plugin_dir, False)
    os.rename(plugin_dir, plugins_folder)

    shutil.copy('build/Manual/htmlhelp/WinMerge.chm', doc_folder)

    shutil.copy('Docs/Users/ReadMe.txt', bin_folder)
    shutil.copy('Docs/Users/ChangeLog.txt', bin_folder)
    shutil.copy('Docs/Users/Contributors.txt', bin_folder)
    shutil.copy('Docs/Users/Files.txt', bin_folder)

def copy_po_files(dest_folder):
    """Copies all PO files to destination folder."""

    lang_folder = 'Src/Languages'
    files = os.listdir(lang_folder)

    for cur_file in files:
        fullpath = os.path.join(lang_folder, cur_file)
        if os.path.isfile(fullpath):
            file_name, file_ext = os.path.splitext(cur_file)
            if (file_ext == '.po'):
                print 'Copying ' + cur_file
                shutil.copy(fullpath, dest_folder)

def usage():
    print 'WinMerge release script.'
    print 'Usage: release [-h] [-v: n]'
    print '  where:'
    print '    -h, --help print this help'
    print '    -v: n, --version= n set release version'
    print '  For example: release -v: 2.7.7.1'

def main(argv):
    version = '0.0.0.0'
    if len(argv) > 0:
        opts, args = getopt.getopt(argv, "hv:", [ "help", "version="])
        
        for opt, arg in opts:
            if opt in ("-h", "--help"):
                usage()
                sys.exit()
            if opt in ("-v", "--version"):
                version = arg
                print "Start building WinMerge release version " + version

    #os.chdir("G:\\WinMerge\\WinMerge_SVN")
    cleanup_build()

    version_folder = 'WinMerge-' + version
    dist_folder = get_and_create_dist_folder(version_folder)
    dist_src_folder = get_src_dist_folder(dist_folder, version_folder)
    svn_export(dist_src_folder)

    set_resource_version(version)
    setup_translations()
    
    build_targets()
    build_manual()
    
    dist_bin_folder = get_and_create_bin_folder(dist_folder, version_folder)
    create_bin_folders(dist_bin_folder, dist_src_folder)


### MAIN ###
if __name__ == "__main__":
    main(sys.argv[1:])
