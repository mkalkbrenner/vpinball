Const PPUC_EVENT_SOURCE_EVENT    = 69 ' "E" VPX/DOF/PUP common event from different system, like
Const PPUC_EVENT_SOURCE_GI       = 71 ' "G" WPC GI
Const PPUC_EVENT_SOURCE_LIGHT    = 76 ' "L"
Const PPUC_EVENT_SOURCE_SOLENOID = 83 ' "S" VPX/DOF/PUP includes flashers
Const PPUC_EVENT_SOURCE_SWITCH   = 87 ' "W" VPX/DOF/PUP
Const PPUC_EVENT_READ_SWITCHES   = 82 ' "R" Read switches command

' Defaults, should be adjusted in table scripts
Dim PPUC_UseLamps = 1
Dim PPUC_UseSolenoids = 1
Dim PPUC_UseGI = 1
Dim PPUC_IOBoardCount = 4
DIM PPUC_ComPort = "Com3"

' Internal state variables
Dim PPUC_CurrentIOBoard = 0

Sub PPUC_Init
	OpenSerial(PPUC_ComPort)
	SetupSerial 115200, 8, 0, 1, True, False
End Sub

Sub PPUC_Exit
	CloseSerial
End Sub

Sub PPUC_SendLightState(ByVal IDPar, ByVal StatePar)
	PPUC_Dispatch PPUC_EVENT_SOURCE_LIGHT, IDPar, StatePar
End Sub

Sub PPUC_SendSolenoidState(ByVal IDPar, ByVal StatePar)
	PPUC_Dispatch PPUC_EVENT_SOURCE_SOLENOID, IDPar, StatePar
End Sub

Sub PPUC_SendGIState(ByVal IDPar, ByVal StatePar)
	PPUC_Dispatch PPUC_EVENT_SOURCE_GI, IDPar, StatePar
End Sub

Sub PPUC_Dispatch(ByVal EventSourcePar, ByVal EventIDPar, ByVal EventStatePar)
  WriteSerial(Array(cbyte(255), cbyte(EventSourcePar), cbyte(0), cbyte(EventIDPar), cbyte(EventStatePar), cbyte(255)))
End Sub


'**************************************************
' Lamps
'**************************************************
Set PPUC_LampCallback = GetRef("PPUC_UpdateLamps")

Sub PPUC_UpdateLamps
     Dim Changed, ii
     Changed = Controller.ChangedLamps
     If IsEmpty(Changed) Then Exit Sub
     On Error Resume Next

     ' iterate over all changed lamps
    For ii = 0 To UBound(Changed)
		PPUC_SendLightState Changed(ii, CHGNO), Changed(ii, CHGSTATE))
    Next
End Sub

'**************************************************
' Solenoids & Flashers
'**************************************************
Set PPUCSolenoidCallback = GetRef("PPUC_UpdateSolenoids")

Sub PPUC_UpdateSolenoids
     Dim Changed, ii
     Changed = Controller.ChangedSolenoids
     If IsEmpty(Changed) Then Exit Sub
     On Error Resume Next

     ' iterate over all changed lamps
    For ii = 0 To UBound(Changed)
		PPUC_SendSolenoidState Changed(ii, CHGNO), Changed(ii, CHGSTATE))
    Next
End Sub

'**************************************************
' WPC GI
'**************************************************
Set PPUCGICallback = GetRef("PPUC_SendGIState")

'**************************************************
' Switches
'**************************************************
Set PPUCSwitchesCallback = GetRef("PPUC_UpdateSwitches")

' read switches from all IO Boards, either all at once or one board per call (timing)
Sub PPUC_UpdateSwitches
    Dim Event(6) As Byte

    ' read from IO board
    PPUC_Dispatch(PPUC_EVENT_READ_SWITCHES, PPUC_CurrentIOBoard, 0)
    ReadSerial(6, Event);
    If Event[1] = PPUC_EVENT_SOURCE_SWITCH And (Event[5] = 0 Or Event[5] = 1) Then
        Controller.Switch(Event[4]) = Event[5]
    End If

    PPUC_CurrentIOBoard = PPUC_CurrentIOBoard + 1
    if PPUC_CurrentIOBoard >= PPUC_IOBoardCount Then PPUC_CurrentIOBoard = 0
End Sub
