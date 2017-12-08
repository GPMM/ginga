<ncl>
  <head>
    <connectorBase>
      <causalConnector id="onBeginSet">
        <connectorParam name="var"/>
        <connectorParam name="duration"/>
        <simpleCondition role="onBegin"/>
        <simpleAction role="set" value="$var" duration="$duration"/>
      </causalConnector>
      <causalConnector id="onEndAttributionSet">
        <connectorParam name="var"/>
        <simpleCondition role="onEndAttribution"/>
        <simpleAction role="set" value="$var"/>
      </causalConnector>
    </connectorBase>
  </head>
  <body>
    <port id="start1" component="dummy"/>
    <port id="start2" component="ctx"/>
    <media id="dummy">
      <property name="size" value="100%,100%"/>
      <property name="background" value="red"/>
      <area id="a1" begin="1s"/>
    </media>
    <context id="ctx">
      <property name="propName" value=""/>
      <port id="start0a" component="time0"/>
      <media id="time0"/>
    </context>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="dummy" interface="a1"/>
      <bind role="set" component="ctx" interface="propName">
        <bindParam name="var" value="newPropValue"/>
      </bind>
    </link>
    <link xconnector="onEndAttributionSet">
      <bind role="onEndAttribution" component="ctx" interface="propName"/>
      <bind role="set" component="dummy" interface="background">
        <bindParam name="var" value="green"/>
      </bind>
    </link>
  </body>
</ncl>