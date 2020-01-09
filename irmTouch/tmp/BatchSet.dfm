object FormBatchSet: TFormBatchSet
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #21442#25968#25209#37327#20462#25913
  ClientHeight = 163
  ClientWidth = 233
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 40
    Top = 11
    Width = 60
    Height = 13
    Caption = #36215#22987#22320#22336#65306
  end
  object Label2: TLabel
    Left = 40
    Top = 38
    Width = 60
    Height = 13
    Caption = #35774#23450#26495#25968#65306
  end
  object Label3: TLabel
    Left = 40
    Top = 65
    Width = 60
    Height = 13
    Caption = #35774#23450#39033#30446#65306
  end
  object Label4: TLabel
    Left = 40
    Top = 92
    Width = 60
    Height = 13
    Caption = #35774#23450#21442#25968#65306
  end
  object ComboBox1: TComboBox
    Left = 106
    Top = 62
    Width = 87
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 0
    Text = #25509#25910#22686#30410'1'
    OnChange = ComboBox1Change
    Items.Strings = (
      #25509#25910#22686#30410'1'
      #25509#25910#22686#30410'2'
      #21457#23556#21151#29575)
  end
  object Edit1: TEdit
    Left = 106
    Top = 8
    Width = 87
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = '1'
  end
  object Edit2: TEdit
    Left = 106
    Top = 35
    Width = 87
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = '5'
  end
  object ComboBox2: TComboBox
    Left = 106
    Top = 89
    Width = 87
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemIndex = 2
    ParentFont = False
    TabOrder = 3
    Text = '2'
    Items.Strings = (
      '0'
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7')
  end
  object Button1: TButton
    Left = 81
    Top = 130
    Width = 75
    Height = 25
    Caption = #35774'  '#32622
    TabOrder = 4
    OnClick = Button1Click
  end
end
