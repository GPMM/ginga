<ncl>
  <head>
    <connectorBase>
      <causalConnector id="onBeginStart_delay">
        <connectorParam name="delay"/>
        <simpleCondition role="onBegin"/>
        <simpleAction role="start" delay="$delay" max="unbounded" qualifier="par"/>
      </causalConnector>
      <causalConnector id="onBeginStart">
        <simpleCondition role="onBegin"/>
        <simpleAction role="start" max="unbounded" qualifier="par"/>
      </causalConnector>
    </connectorBase>
    <regionBase>
      <region id="reg1" width="50%" height="50%"/>
      <region id="reg2" left="50%" width="50%" height="50%"/>
      <region id="reg3" top="50%" width="50%" height="50%"/>
      <region id="reg4" top="50%" left="50%" width="50%" height="50%"/>
      <region id="reg5"/>
    </regionBase>
    <descriptorBase>
      <descriptor id="desc1" region="reg1"/>
      <descriptor id="desc2" region="reg2"/>
      <descriptor id="desc3" region="reg3"/>
      <descriptor id="desc4" region="reg4"/>
      <descriptor id="desc5" region="reg5" explicitDur="5s"/>
    </descriptorBase>
  </head>
  <body>
    <port id="start" component="text"/>
    <port id="start2" component="text2"/>
    <port id="start3" component="text3"/>
    <port id="start4" component="text4"/>
    <media id="text" src="samples/text.txt" descriptor="desc1">
      <area id="segMpegV" begin="5s"/>
      <property name="fontColor" value="#000000" />
      <property name="backgroundColor" value="rgb(255,255,0)" />
      <property name="fontFamily" value="Courier" />
      <property name="horzAlign" value="center" />
      <property name="vertAlign" value="middle" />
      <property name="fontStyle" value="normal" />
      <property name="fontSize" value="18px" />
      <property name="fontWeight" value="normal" />
      <property name="fontVariant" value="small-caps" />
    </media>
    <media id="text2" src="samples/text.txt" descriptor="desc2">
      <property name="fontColor" value="rgb(255,0,0)" />
      <property name="backgroundColor" value="#0F0" />
      <property name="fontFamily" value="Arial" />
      <property name="horzAlign" value="left" />
      <property name="vertAlign" value="bottom" />
      <property name="fontStyle" value="italic" />
      <property name="fontSize" value="12pt" />
      <property name="fontWeight" value="bold" />
      <property name="fontVariant" value="small-caps" />
    </media>
    <media id="text3" src="samples/text.txt" descriptor="desc3">
      <property name="fontColor" value="#E6F732" />
      <property name="backgroundColor" value="#000055" />
      <property name="fontFamily" value="Times" />
      <property name="horzAlign" value="justified" />
      <property name="vertAlign" value="top" />
      <property name="fontStyle" value="italic" />
      <property name="fontSize" value="22px" />
      <property name="fontWeight" value="bold" />
      <property name="fontVariant" value="small-caps" />
    </media>
    <media id="text4" src="samples/text.txt" descriptor="desc4">
      <property name="fontColor" value="#000000" />
      <property name="backgroundColor" value="#e5e5e5" />
      <property name="fontFamily" value="Georgia" />
      <property name="horzAlign" value="right" />
      <property name="vertAlign" value="middle" />
      <property name="fontStyle" value="italic" />
      <property name="fontSize" value="16" />
      <property name="fontWeight" value="normal" />
      <property name="fontVariant" value="small-caps" />
    </media>
    <media id="mpegv1" type="text/mpegv" descriptor="desc5">
      <property name="SEtype" value="windType" />
      <property name="location" value="left:*:*" />
      <property name="intensity" value="3.0" />
    </media>
    <link id="lText" xconnector="onBeginStart_delay">
      <bind role="onBegin" component="text" interface="segMpegV"/>
      <bind role="start" component="mpegv1"/>
    </link>
  </body>
</ncl>
