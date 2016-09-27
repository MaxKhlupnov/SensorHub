WITH 
TemperatureSummary AS (
SELECT
    DeviceId,
    MAX (CAST(Time AS Datetime)) AS [EntryTime],  
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
    MAX (CAST(Time AS Datetime)) AS [EntryTime],  
    AVG (CAST(Value AS FLOAT)) AS [AvgHumidity],
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
    
  --  SELECT * INTO temperatureAlerts FROM TemperatureSummary
--    SELECT * INTO humidityAlerts FROM HumiditySummary
    
  SELECT T.EntryTime, T.DeviceId, T.AvgTemperature, T.CountTemperature, H.EntryTime, H.AvgHumidity,
  H.CountHumidity 
  FROM TemperatureSummary T 
  JOIN  HumiditySummary H 
  ON T.DeviceId = H.DeviceId
  AND    DATEDIFF(minute,T,H) BETWEEN 0 AND 15
     