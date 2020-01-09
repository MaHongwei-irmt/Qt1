object MDIChild: TMDIChild
  Left = 197
  Top = 117
  Caption = 'MDI Child'
  ClientHeight = 468
  ClientWidth = 830
  Color = clBtnFace
  ParentFont = True
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Chart1: TChart
    Left = 0
    Top = 0
    Width = 830
    Height = 468
    Border.Width = 0
    LeftWall.Pen.Width = 0
    Legend.Visible = False
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    BottomAxis.Axis.Color = clSilver
    BottomAxis.Axis.Width = 0
    BottomAxis.LabelStyle = talValue
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.Axis.Color = clSilver
    LeftAxis.Axis.Width = 0
    LeftAxis.Grid.Width = 0
    LeftAxis.LabelsFormat.Frame.Width = 0
    LeftAxis.LabelsFormat.Margins.Left = 0
    LeftAxis.LabelsFormat.Margins.Top = 0
    LeftAxis.LabelsFormat.Margins.Right = 0
    LeftAxis.LabelsFormat.Margins.Bottom = 0
    LeftAxis.LabelStyle = talValue
    LeftAxis.Maximum = 255.000000000000000000
    LeftAxis.MinorTickCount = 0
    LeftAxis.Ticks.Width = 0
    Panning.MouseWheel = pmwNone
    RightAxis.Visible = False
    Shadow.Visible = False
    TopAxis.Visible = False
    View3D = False
    View3DOptions.Orthogonal = False
    View3DWalls = False
    Zoom.MinimumPixels = 15
    Zoom.MouseWheel = pmwNormal
    Zoom.Pen.Width = 4
    Align = alClient
    BevelOuter = bvNone
    PopupMenu = MainForm.PopupMenu1
    TabOrder = 0
    DefaultCanvas = 'TGDIPlusCanvas'
    PrintMargins = (
      15
      21
      15
      21)
    ColorPaletteIndex = 13
  end
end
