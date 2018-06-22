/* Private Functions */
let badRequest = Express.Response.StatusCode.BadRequest;

let ok = Express.Response.StatusCode.Ok;

let beltMapToJson = map =>
  Belt.Map.String.toArray(map) |> Js.Dict.fromArray |> Js.Json.object_;

let errorMapToJson = map =>
  Belt.Map.String.map(map, x =>
    switch (x) {
    | None => Js.Json.null
    | Some(str) => Js.Json.string(str)
    }
  )
  |> beltMapToJson;

let sendJson = (res, statusCode, json) =>
  res
  |> Express.Response.status(statusCode)
  |> Express.Response.sendJson(json)
  |> Js.Promise.resolve;

let error = (_, res, err) =>
  sendJson(res, badRequest, err |> errorMapToJson);

let success = (promisified, decoder, encoder, req, res, data) =>
  beltMapToJson(data)
  |. decoder
  |. promisified(req)
  |. (p => p |> Js.Promise.then_(j => sendJson(res, ok, j |> encoder)));

let middleware = (awesomizer, success, error) =>
  (. _, req, res) =>
    Express.Request.asJsonObject(req)
    |> awesomizer
    |> Awesomize.Result.fold(error(req, res), success(req, res))
    |> Js.Promise.then_(r => r);

/* Public Functions */
/* @TODO - Awesomizer needs to support a context and this integration needs
   to accept a function which can generate that context from the request
   object. */
let make = (~schema, ~decoder, ~encoder, ~handler, _, req, res) =>
  Express.Request.asJsonObject(req)
  |> Awesomize.make(schema)
  |> Awesomize.Result.fold(
       err => sendJson(res, badRequest, errorMapToJson(err)),
       data =>
         data
         |> beltMapToJson
         |> decoder
         |> handler(req)
         |> Js.Promise.then_(json => sendJson(res, ok, encoder(json))),
     )
  |> Js.Promise.then_(result => result);

module Promise = {
  let make = (~schema, ~decoder, ~encoder, ~handler) => {
    let awesomizer = Awesomize.make(schema);
    let promisified = (data, req) => handler(req, data);
    let success = success(promisified, decoder, encoder);

    middleware(awesomizer, success, error);
  };
};

module Future = {
  let make = (~schema, ~decoder, ~encoder, ~handler) => {
    let awesomizer = Awesomize.make(schema);

    let promisified = (decoded, req) =>
      Js.Promise.make((~resolve, ~reject) =>
        handler(req, decoded)
        |. Future.mapOk(result => resolve(. result))
        |. Future.mapError(err => reject(. err))
        |. ignore
      );

    let success = success(promisified, decoder, encoder);

    middleware(awesomizer, success, error);
  };
};
