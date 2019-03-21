<?xml version="1.0" encoding="ISO-8859-1"?>
<ncl id="preparationExample" xmlns="http://www.ncl.org.br/NCL3.0/EDTVProfile">
	<head>
		<regionBase>
			<region id="regLoading" width="100%" height="100%" zIndex="1"/>
		</regionBase>

		<descriptorBase>
			<descriptor id="descSE" explicitDur="2s"/>
			<descriptor id="descLoading" region="regLoading"/>
		</descriptorBase>

		<connectorBase>
			<causalConnector id="onBeginStartPreparation">
				<simpleCondition role="onBegin"/>
				<simpleAction role="startPreparation"/>
			</causalConnector>
			<causalConnector id="onEndPreparationStopStart">
				<simpleCondition role="onEndPreparation"/>
				<simpleAction role="stop"/>
				<simpleAction role="start"/>
			</causalConnector>
		</connectorBase>
	</head>
	<body>
		<port id="port1" component="loading"/>

		<media id="loading" src="samples/clock.ogv" descriptor="descLoading"/>
    <media id="se" type="application/x-sensoryEffect" descriptor="descSE"/>

		<link xconnector="onBeginStartPreparation">
			<bind role="onBegin" component="loading"/>
			<bind role="startPreparation" component="se"/>
		</link>

		<link xconnector="onEndPreparationStopStart">
			<bind role="onEndPreparation" component="se"/>
			<bind role="stop" component="loading"/>
			<bind role="start" component="se"/>
		</link>
	</body>
</ncl>
