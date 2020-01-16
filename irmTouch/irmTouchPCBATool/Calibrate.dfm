object FormCalibrate: TFormCalibrate
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'FormCalibrate'
  ClientHeight = 426
  ClientWidth = 670
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  WindowState = wsMaximized
  OnKeyPress = FormKeyPress
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ImageCalibrate: TImage
    Left = 0
    Top = 0
    Width = 670
    Height = 426
    Align = alClient
    OnDblClick = ImageCalibrateDblClick
    ExplicitWidth = 497
    ExplicitHeight = 289
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 240
    Top = 176
  end
end
