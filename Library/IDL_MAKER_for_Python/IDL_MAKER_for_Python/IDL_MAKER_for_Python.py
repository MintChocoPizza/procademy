# -*- coding: utf-8 -*-

###### pip install parse ####
from ctypes import Array
from re import A, S, T
from parse import *

KeyWordTable = ["#PACKETNUM",  "#NOBUFF"]

PACKETNUM : int = 0

ProxyCppDefine : str        = "OreoPizza::Proxy::"
StubCppDefine : str         = "OreoPizza::Stub::"
StubCoverDefine : str       = "Message_" 
StubConverParam : str       = "(char* Message)"

PacketDefineHeaderStr : str = ""
ProxyHeaderStr : str = ""
ProxyCppStr : str = ""
StubHeaderStr: str = "\tprivate: \tvoid ProcessRecvMessage(); \n\n"
StubCppStr : str = ""
StubCppSwitchStr : str = ""

def WritePacketDefineHeader():
    global PacketDefineHeaderStr
    ########################################################
    # ./PacketDefine.h 파일 작성 
    PacketDefineHeader = open("./PacketDefine.h", "w", encoding="utf-16") 
    PacketDefineHeader.write("#ifndef __PACKET_DEFINE__ \n")
    PacketDefineHeader.write("#define __PACKET_DEFINE__ \n\n") 
    PacketDefineHeader.write(PacketDefineHeaderStr)
    PacketDefineHeader.write("\n#endif")
    PacketDefineHeader.close() 
    
def WriteProxyHeader():
    global ProxyHeaderStr
    
    ########################################################
    # ./Proxy.h 파일 작성 
    ProxyHeader = open("./Proxy.h", "w", encoding="utf-16")
    ProxyHeader.write("#ifndef __PROXY_H__ \n")
    ProxyHeader.write("#define __PROXY_H__ \n\n")

    ProxyHeader.write("namespace OreoPizza \n")
    ProxyHeader.write("{ \n")
    
    ProxyHeader.write("\tclass Proxy \n")
    ProxyHeader.write("\t{ \n")
    
    ProxyHeader.write(ProxyHeaderStr)
    
    ProxyHeader.write("\t}; \n")

    ProxyHeader.write("} \n")
    
    ProxyHeader.write("\n#endif")
    ProxyHeader.close()

def WriteProxyCpp():
    global ProxyCppStr


    ########################################################
    # ./Proxy.cpp 파일 작성
    ProxyCpp = open("./Proxy.cpp", "w", encoding="utf-16")
    ProxyCpp.write("#include \"proxy.h\" \n")

    ########################################################
    # 직렬화 버퍼 연결
    ProxyCpp.write("#include \"SerializeBuffer.h\" \n")

    
    ########################################################
    # 네트워크 라이브러리랑 같이 맞물려 있어야 함.
    # But 현재 네트워크 라이브러리가 없어서 일단 건너 뛴다. 

    ProxyCpp.write("\n")
    ProxyCpp.write(ProxyCppStr)
    ProxyCpp.close()

def WriteStubHeader():
    global StubHeaderStr

    ########################################################
    # ./stup.h 파일 작성
    StubHeader = open("./Stub.h", "w", encoding="utf-16")
    StubHeader.write("#ifndef __STUB_H__ \n")
    StubHeader.write("#define __STUB_H__ \n\n")
    
    StubHeader.write("namespace OreoPizza \n")
    StubHeader.write("{ \n")
    
    StubHeader.write("\tclass Stub \n")
    StubHeader.write("\t{ \n")
    
    # 내가 작성한 내용이 들어간다.
    StubHeader.write(StubHeaderStr)

    StubHeader.write("\t}; \n")

    StubHeader.write("} \n")

    StubHeader.write("#endif \n")
    StubHeader.close()
 
def WriteStubCpp():
    global StubCppStr
    

    StubCpp = open("./Stub.cpp", "w", encoding="utf-16")
    StubCpp.write("#include \"Stub.h\" \n")

    ########################################################
    # 직렬화 버퍼 연결
    StubCpp.write("#include \"SerializeBuffer.h\" \n")

    StubCpp.write("\n")
    # 스위치문 작성
    WriteStubProcessRecvMessage(StubCpp)

    # 실제 함수 작성
    StubCpp.write(StubCppStr)

    StubCpp.close()    

def WriteStubProcessRecvMessage(StubCpp):
    global StubCppSwitchStr

    StubCpp.write("void OreoPizza::Stub::ProcessRecvMessage() \n{ \n")
    
    # 메시지에서 PacketType 뽑아내기
    #
    
    StubCpp.write(StubCppSwitchStr)

    StubCpp.write("\tdefault: \n")
    # 이상한 타입의 예외처리~~~~
    StubCpp.write("\t\tbreak; \n")
    StubCpp.write("\t} \n")
    StubCpp.write("} \n\n")

def SaveFile():


    ########################################################
    # ./PacketDefine.h 파일 작성 
    WritePacketDefineHeader()
    
    ########################################################
    # ./Proxy.h 파일 작성 
    WriteProxyHeader()

    ########################################################
    # ./Proxy.cpp 파일 작성
    WriteProxyCpp()

    ########################################################
    # ./stup.h 파일 작성
    WriteStubHeader()

    ########################################################
    # ./stup.cpp 파일 작성
    WriteStubCpp()
    return



def removeKeyWord(Param):
    retStr = "("

    TempParam = Param.split(",")
    for subTempParam in TempParam:
        for subString in KeyWordTable:
            subTempParam = subTempParam.replace(subString, "").strip()
        retStr += subTempParam + ", "
    
    retStr = retStr.rstrip(', ')
    retStr += ")"

    return retStr

def PushBuff(Param):
    global KeyWordTable

    TempStr = "SerializeBuffer sb"
    
    TempParam = Param.split(",")

    for subTempParam in TempParam:
        subTempParam = subTempParam.strip()
        if MyKeyWord(subTempParam) != -1:
            words = subTempParam.split()
            TempStr += " << " + words[-1]

    TempStr += ";"
    return TempStr
    
def MyKeyWord(line : str, option = None):
    global PACKETNUM
    global KeyWordTable

    line = line.strip();

    for subString in KeyWordTable:
        if line.find(subString) != -1:
            if subString == "#PACKETNUM":
                KeyWord, Value = line.split(" ")
                PACKETNUM = int(Value)
                return 
            if subString == "#NOBUFF":
                return -1
    
    return



def SetProxyDefine(FuncName : str):
    global PACKETNUM
    global PacketDefineHeaderStr

    PacketDefineHeaderStr += "#define " + "df" + FuncName + "\t\t\t\t\t\t" + str(PACKETNUM) + " \n"
    return 

def SetProxyHeader(DataType, FuncName, Param):
    global ProxyHeaderStr
    global KeyWordTable
    
    ProxyHeaderStr += "\t\tpublic: \t\t" + DataType + " " + FuncName 

    # 파라미터에서 키워드를 제거하여 만들어야 한다.
    ProxyHeaderStr += removeKeyWord(Param) + "; \n"

    return

def SetProxyCpp(DataType, FuncName, Param) :
    global ProxyCppStr
    global ProxyCppDefine
    global KeyWordTable
    

    ProxyCppStr += DataType + " " + ProxyCppDefine + FuncName 

    # 파라미터에서 키워드를 제거한다. 
    ProxyCppStr += removeKeyWord(Param) + "\n"
        
    ProxyCppStr += "{ \n"

    ########################################################
    # 직렬화 버퍼에 넣는다.
    ProxyCppStr += "\t" + PushBuff(Param) + "\n"
    
    ########################################################
    # Send를 한다. 
    

    ProxyCppStr += "} \n\n"
    return 
    
def SetStubHeader(DataType, FuncName, Param) :
    global StubHeaderStr 
    global StubCoverDefine
    global KeyWordTable
    global StubConverParam
    
    # private 로 이루어진 함수가 메시지를 받아서 메시지를 뜯어내고 
    # protected 로 이루어진 가상함수가 파라미터로 값을 받아서 알아서 처리한다.
    StubHeaderStr += "\tprivate: \tvoid " + StubCoverDefine + FuncName + StubConverParam + "; \n"
    StubHeaderStr += "\tprotected: \t" + DataType + " " + FuncName 
    
    StubHeaderStr += removeKeyWord(Param) + "; \n\n"

    return
    
def SetStubCpp(DataType, FuncName, Param) :
    global StubCppStr
    global StubCppDefine
    global KeyWordTable
    global StubCoverDefine
    global StubConverParam
    
    ########################################################
    # private 껍데기 함수
    StubCppStr += "void " + StubCppDefine + StubCoverDefine + FuncName + StubConverParam + " \n"
    StubCppStr += "{ \n"

    # 메시지를 뜯어서 가상함수에 파라미터로 넘긴다. 
    StubCppStr += "\t" + FuncName + "(); \n"

    StubCppStr += "} \n\n"
    


    ########################################################
    # 사용자가 실제 사용할 함수 
    StubCppStr +=  DataType + " " + StubCppDefine + FuncName 
    
    StubCppStr += removeKeyWord(Param) + "\n"

    StubCppStr += "{ \n"
    StubCppStr += "\t return "
    
    if DataType == "void":
        StubCppStr += "; \n"
    else :
        StubCppStr += "false; \n"
        
    StubCppStr += "} \n\n"
        

    

    return 
    
def SetStubCppSwitch(DataType, FuncName, Param):
    global StubCppSwitchStr
    global KeyWordTable
    global StubCoverDefine
    global StubConverParam
    

    StubCppSwitchStr += "\tcase " + "df" + FuncName.upper() + ": \n"
    StubCppSwitchStr += "\t\t" + StubCoverDefine + FuncName + StubConverParam + "; \n"
    StubCppSwitchStr += "\t\tbreak; \n"

    

    return

    
    

def ParsingFunc(line : str):
    global PACKETNUM

    result = parse("{DataType} {FuncName}({Param})", line)

    DataType = result["DataType"]
    FuncName = result["FuncName"].strip()
    Param   = result["Param"]
    

    # 1. PacketDefine.h 파일에 작성할 값을 작성한다. 
    SetProxyDefine(FuncName.upper())

    # 2. Proxy.h 파일에 작성할 값을 작성한다. 
    SetProxyHeader(DataType, FuncName, Param)
    
    # 3. Proxy.cpp 파일에 작성할 값을 작성한다. 
    SetProxyCpp(DataType, FuncName, Param)

    # 4. Stub.h 파일에 작성할 값을 작성한다. 
    SetStubHeader(DataType, FuncName, Param)

    # 5. Stub.cpp 파일에 작성할 값을 작성한다. 
    SetStubCpp(DataType, FuncName, Param)

    # 6. Stub.cpp 파일에 있는 switch 문을 작성한다.
    SetStubCppSwitch(DataType, FuncName, Param)
    
    PACKETNUM += 1

    

    


def main():

    File = open("./IDL.cnf", 'r', encoding = 'utf-8')
    while True:
        line = File.readline()
        if not line: break 

        # 엔터를 누른 빈 줄이면 건너뛴다.
        if line == '\n':
            continue
        # '//' 로 시작하면 주석이다. 
        elif line[0:2] == "//":
            continue
        # '#' 로 시작하면 내가 등록한 나만의 키워드이다.
        elif line[0] == "#":
            MyKeyWord(line)
        # 엔터를 누른 빈 줄이면 건너뛴다.
        else:
            ParsingFunc(line.strip())
            
    File.close()



    

if __name__ == "__main__":
    main()
    SaveFile()
