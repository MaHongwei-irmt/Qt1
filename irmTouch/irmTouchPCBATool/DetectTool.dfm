object DetectTool: TDetectTool
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'FactoryTool'
  ClientHeight = 688
  ClientWidth = 1359
  Color = clBtnFace
  DockSite = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  WindowState = wsMaximized
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnDblClick = FormDblClick
  DesignSize = (
    1359
    688)
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1359
    Height = 209
    Align = alTop
    Caption = 'Panel1'
    DockSite = True
    TabOrder = 1
    ExplicitWidth = 1012
  end
  object RichEdit1: TRichEdit
    Left = 280
    Top = 272
    Width = 185
    Height = 89
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      'RichEdit1')
    ParentFont = False
    TabOrder = 2
    Zoom = 100
  end
  object ctlInfoShow: TRichEdit
    Left = 0
    Top = 17
    Width = 1359
    Height = 153
    Anchors = [akLeft, akTop, akRight]
    DragKind = dkDock
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      'ctlInfoShow')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
    Zoom = 100
    ExplicitWidth = 1012
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 120
    Top = 528
  end
end
