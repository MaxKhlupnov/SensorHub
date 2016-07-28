WITH 
TemperatureSummary AS (
SELECT
    DeviceId,
    AVG (CAST(Value AS FLOAT)) AS [AvgTemperature],  
    COUNT(*) AS [CountTemperature],  
    15.0 AS TimeframeMinutes 
FROM
    [StreamData]
WHERE
    [ValueLabel] LIKE 'Temperature' AND 
    [Type] LIKE 'Decimal' AND
    [Value] IS NOT NULL
    
GROUP BY
    DeviceId, 
    SlidingWindow (mi, 15)
    ),
 HumiditySummary AS
(
SELECT
    DeviceId,
    MIN (CAST(Time AS DATETIME)) AS EntryTime,
    AVG (CAST(Value AS FLOAT)) AS [AverageHumidity],
    COUNT(*) AS [CountHumidity],  
    15.0 AS TimeframeMinutes 
FROM
    [StreamData]
WHERE
    [ValueLabel] LIKE 'Relative Humidity' AND 
    [Type] LIKE 'Decimal' AND
    [Value] IS NOT NULL
    
GROUP BY
    DeviceId, 
    SlidingWindow (mi, 15)
)
    SELECT * INTO temperatureAlerts FROM TemperatureSummary
    SELECT * INTO humidityAlerts FROM HumiditySummary             