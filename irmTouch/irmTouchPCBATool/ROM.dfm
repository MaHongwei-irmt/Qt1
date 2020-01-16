object FormROM: TFormROM
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'ROM View'
  ClientHeight = 426
  ClientWidth = 431
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 76
    Height = 17
    Caption = 'Main Flash:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -14
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 431
    Top = 8
    Width = 65
    Height = 17
    Caption = 'EEPROM:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -14
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 431
    Top = 289
    Width = 62
    Height = 17
    Caption = 'CONFIG:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -14
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object StringGrid1: TStringGrid
    Left = 8
    Top = 31
    Width = 417
    Height = 386
    ColCount = 10
    DefaultColWidth = 40
    DefaultRowHeight = 20
    FixedCols = 0
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
    ColWidths = (
      68
      40
      40
      40
      40
      40
      40
      40
      40
      156)
    RowHeights = (
      20
      20
      20
      20
      20)
  end
  object StringGrid3: TStringGrid
    Left = 431
    Top = 312
    Width = 417
    Height = 105
    ColCount = 10
    DefaultColWidth = 40
    DefaultRowHeight = 20
    FixedCols = 0
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 1
    Visible = False
    ColWidths = (
      68
      40
      40
      40
      40
      40
      40
      40
      40
      156)
    RowHeights = (
      20
      20
      20
      20
      20)
  end
  object Button1: TButton
    Left = 581
    Top = 440
    Width = 75
    Height = 25
    Caption = 'Refresh'
    TabOrder = 2
    Visible = False
  end
  object Button2: TButton
    Left = 662
    Top = 440
    Width = 75
    Height = 25
    Caption = 'Fill Random'
    TabOrder = 3
    Visible = False
  end
  object StringGrid2: TStringGrid
    Left = 431
    Top = 31
    Width = 417
    Height = 386
    ColCount = 10
    DefaultColWidth = 40
    DefaultRowHeight = 20
    FixedCols = 0
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 4
    ColWidths = (
      68
      40
      40
      40
      40
      40
      40
      40
      40
      156)
    RowHeights = (
      20
      20
      20
      20
      20)
  end
end
