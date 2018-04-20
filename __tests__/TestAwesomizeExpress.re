open Jest;

type t = {clientId: int};

let req: Express.Request.t = [%raw "{clientId: 1}"];

let res: Express.Response.t = [%raw
  "{ status: (code) => ({ json: () => ({clientId: code === 200 ? 1 : -1}) }) }"
];

external complete : Express.complete => Js.Json.t = "%identity";

external castExpressToJson : Express.Request.t => Js.Json.t = "%identity";

describe("Awesomize Express", () =>
  describe("make", () => {
    let schema = [|
      (
        "clientId",
        {
          read: Awesomize.Read.key("clientId"),
          sanitize: None,
          validate: [Awesomize.Validator.required],
          normalize: None,
        }: Awesomize.definition,
      ),
    |];
    let decoder = (_) => ();
    let handler = (req, _) => Js.Promise.resolve();
    let encoder = (_) => castExpressToJson(req);
    testPromise("success", () =>
      AwesomizeExpress.make(
        ~schema,
        ~decoder,
        ~encoder,
        ~handler,
        (),
        req,
        res,
      )
      |> Js.Promise.then_(result => {
           let temp = {
             clientId:
               Json.Decode.field(
                 "clientId",
                 Json.Decode.int,
                 complete(result),
               ),
           };
           (
             switch (temp) {
             | {clientId: 1} => pass
             | _ => fail("not an expected result")
             }
           )
           |> Js.Promise.resolve;
         })
    );
  })
);
