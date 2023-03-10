SELECT date AS date,
       temperature AS temperature,
       humidity AS humidity,
       light AS light,
       magnetic AS magnetic,
       gas AS gas,
       noiselevel AS noise,
       cycle AS current_cycle
FROM sensor_data
WHERE date >= NOW() - INTERVAL '4 minutes';
