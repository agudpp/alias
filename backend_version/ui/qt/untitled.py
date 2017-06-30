  
# class Mywin(MyFrame): 
#    def __init__(self, parent, title): 
#       super(Mywin, self).__init__(parent, title = title,size = (350,250))
        
#       # panel = wx.Panel(self) 
#       # vbox = wx.BoxSizer(wx.VERTICAL) 
         
#       # hbox1 = wx.BoxSizer(wx.HORIZONTAL) 
#       # l1 = wx.StaticText(panel, -1, "Text Field") 
        
#       # hbox1.Add(l1, 1, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5) 
#       # self.t1 = wx.TextCtrl(panel) 
        
#       # hbox1.Add(self.t1,1,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5) 
#       self.text_ctrl_1.Bind(wx.EVT_TEXT,self.OnKeyTyped) 
#       self.text_ctrl_1.Bind(wx.EVT_KEY_DOWN, self.onKeyDown)

#       # vbox.Add(hbox1) 
        
#       # hbox2 = wx.BoxSizer(wx.HORIZONTAL)
#       # l2 = wx.StaticText(panel, -1, "password field") 
        
#       # hbox2.Add(l2, 1, wx.ALIGN_LEFT|wx.ALL,5) 
#       # self.t2 = wx.TextCtrl(panel,style = wx.TE_PASSWORD) 
#       # self.t2.SetMaxLength(5) 

#       # hbox2.Add(self.t2,1,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5) 
#       # vbox.Add(hbox2) 
#       # self.t2.Bind(wx.EVT_TEXT_MAXLEN,self.OnMaxLen)
        
#       # hbox3 = wx.BoxSizer(wx.HORIZONTAL) 
#       # l3 = wx.StaticText(panel, -1, "Multiline Text") 
        
#       # hbox3.Add(l3,1, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5) 
#       # self.t3 = wx.TextCtrl(panel,size = (200,100),style = wx.TE_MULTILINE) 
        
#       # hbox3.Add(self.t3,1,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5) 
#       # vbox.Add(hbox3) 
#       # self.t3.Bind(wx.EVT_TEXT_ENTER,self.OnEnterPressed)  
        
#       # hbox4 = wx.BoxSizer(wx.HORIZONTAL) 
#       # l4 = wx.StaticText(panel, -1, "Read only text") 
        
#       # hbox4.Add(l4, 1, wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5) 
#       # self.t4 = wx.TextCtrl(panel, 
#       #                       value = "ReadOnly Text",
#       #                       style = wx.TE_READONLY|wx.TE_CENTER) 
            
#       # hbox4.Add(self.t4,1,wx.EXPAND|wx.ALIGN_LEFT|wx.ALL,5) 
#       # vbox.Add(hbox4) 
#       # panel.SetSizer(vbox) 
        
#       self.Centre() 
#       self.Show() 
#       self.Fit()  

#    def onKeyDown(self, event):
#       keycode = event.GetKeyCode()
#       if keycode == wx.WXK_TAB:
#           # self.process_text(event=None)
#           # event.EventObject.Navigate()
#           print ("tab pressed")
#           return False
#       event.Skip()
        
#    def OnKeyTyped(self, event): 
#       theStr = event.GetString()
#       print("xxx: ", theStr)
#       resp = sendReq(builldQuery(theStr))
#       self.showCurrTags(resp["matched_tags"])
#       self.showExpTags(resp["expanded_tags"])
#       # we need here to get the current tags and the expansion
        
#    def OnEnterPressed(self,event): 
#       print ("Enter pressed" )
        
#    def OnMaxLen(self,event): 
#       print ("Maximum length reached" )


#    def showExpTags(self, tagList):
#       endString = ""
#       for t in tagList:
#         endString += t + "\n"
      
#       self.expanded_tags.SetLabel(endString)

#    def showCurrTags(self, tagList):
#       endString = ""
#       for t in tagList:
#         endString += t + "\n"
      
#       self.current_tags.SetLabel(endString)
