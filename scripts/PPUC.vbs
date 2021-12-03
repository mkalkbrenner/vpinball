Const PPUC_EVENT_SOURCE_LIGHT    = 76 'L
Const PPUC_EVENT_SOURCE_SOLENOID = 83 'S

Sub PPUC_Init(ComPort)
	OpenSerial(ComPort)
	SetupSerial 115200, 8, 0, 1, True, False
End Sub

Sub PPUC_Exit
	CloseSerial
End Sub

Sub PPUC_Dispatch(ByVal EventSourcePar, ByVal EventIDPar, ByVal EventStatePar)
  WriteSerial(Array(cbyte(255), cbyte(EventSourcePar), cbyte(0), cbyte(EventIDPar), cbyte(EventStatePar), cbyte(255)))
End Sub

