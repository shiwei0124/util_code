#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
from xml.dom import minidom
import codecs

def search_file_by_ext(ext):
    file_list = []
    curdir = os.getcwd()
    files = os.listdir(curdir)
    for _file in files:
        file_name, file_ext = os.path.splitext(_file)
        if file_ext == ext:
            file_list.append(_file)
    return file_list

def change_vcproj(projfile):
    doc = minidom.parse(projfile)
    NodeList = doc.getElementsByTagName("VisualStudioProject")
    Node = NodeList[0]
    AttrNode = Node.getAttributeNode("Name")
    projname = AttrNode.value
    NodeList = doc.getElementsByTagName("Configuration")
    for Node in NodeList:
        AttrNode = Node.getAttributeNode("Name")
        if AttrNode.value.find("Debug") != -1:
            print "Modify Debug Configuration"
            OutputDir = Node.getAttributeNode("OutputDirectory")
            OutputDir.value = "..\\..\\bin\\vs2008_debug"
            IntermediateDir = Node.getAttributeNode("IntermediateDirectory")
            IntermediateDir.value = "..\\..\\obj\\%s\\vs2008_debug"%projname
            ChildNodeList = Node.getElementsByTagName("Tool")
            for ChildNode in ChildNodeList:
                NameAttr = ChildNode.getAttributeNode("Name")
                if NameAttr.value == "VCCLCompilerTool":
                    IncludeDir = ChildNode.getAttributeNode("AdditionalIncludeDirectories")
                    if IncludeDir != None:
                        IncludeDir.value = "..\\..\\public\\include;" + IncludeDir.value
                        print "Append Debug IncludeDir"
                    else:
                        ChildNode.setAttribute("AdditionalIncludeDirectories", "..\\..\\public\\include;")
                        print "Create Debug IncludeDir"
                if NameAttr.value == "VCLinkerTool":
                    LibraryDir = ChildNode.getAttributeNode("AdditionalLibraryDirectories")
                    if LibraryDir != None:
                        LibraryDir.value = "..\\..\\public\\lib\\win32_debug;" + LibraryDir.value
                        print "Append Debug LibraryDir"
                    else:
                        ChildNode.setAttribute("AdditionalLibraryDirectories", "..\\..\\public\\lib\\win32_debug;")
                        print "Create Debug LibraryDir"
                    
        if AttrNode.value.find("Release") != -1:
            print "Modify Release Configuration"
            OutputDir = Node.getAttributeNode("OutputDirectory")
            OutputDir.value = "..\\..\\bin\\vs2008_release"
            IntermediateDir = Node.getAttributeNode("IntermediateDirectory")
            IntermediateDir.value = "..\\..\\obj\\%s\\vs2008_release"%projname
            ChildNodeList = Node.getElementsByTagName("Tool")
            for ChildNode in ChildNodeList:
                NameAttr = ChildNode.getAttributeNode("Name")
                if NameAttr.value == "VCCLCompilerTool":
                    IncludeDir = ChildNode.getAttributeNode("AdditionalIncludeDirectories")
                    if IncludeDir != None:
                        IncludeDir.value = "..\\..\\public\\include;" + IncludeDir.value
                        print "Append Release IncludeDir"
                    else:
                        ChildNode.setAttribute("AdditionalIncludeDirectories", "..\\..\\public\\include;")
                        print "Create Release IncludeDir"
                if NameAttr.value == "VCLinkerTool":
                    LibraryDir = ChildNode.getAttributeNode("AdditionalLibraryDirectories")
                    if LibraryDir != None:
                        LibraryDir.value = "..\\..\\public\\lib\\win32_release;" + LibraryDir.value
                        print "Append Release LibraryDir"
                    else:
                        ChildNode.setAttribute("AdditionalLibraryDirectories", "..\\..\\public\\lib\\win32_release;")
                        print "Create Release LibraryDir"
    return doc

def change_encoding(projfile):
    file_object_r = open(projfile,'r')
    file_xml = file_object_r.read()
    if file_xml.find('<?xml version="1.0" encoding="gb2312"?>') != -1:
        file_xml = file_xml.replace('<?xml version="1.0" encoding="gb2312"?>','<?xml version="1.0" encoding="utf-8"?>')
        file_xml = unicode(file_xml,encoding='gbk').encode('utf-8')
        file_object_r.close()
        file_object_w = open(projfile, 'w')
        file_object_w.write(file_xml)
        file_object_w.close()
        print "%s: convert gb2312 to utf-8."%projfile
    else:
        file_object_r.close()

def backup_vcproj(projfile):
    os.popen("copy %s %s.bak"%(projfile, projfile))

def resave_vcproj(vcproj):
    f = codecs.open(projfile,'w','utf-8')  
    vcproj.writexml(f,encoding = 'utf-8')  
    f.close()  


if __name__ == "__main__":
    files = search_file_by_ext(".vcproj")
    for projfile in files:
        print "*************configure vcproj: %s**************"%projfile
        backup_vcproj(projfile)
        change_encoding(projfile)
        vcproj = change_vcproj(projfile)
        resave_vcproj(vcproj)
    print u"请按任意键继续..."
    raw_input()
    
    '''
    except Exception as e:
        print "change vcproj failed, error: %s."%e
        raise e
    else:
        print "change vcproj successed."
    '''
